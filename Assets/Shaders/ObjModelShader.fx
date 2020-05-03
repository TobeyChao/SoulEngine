struct Light
{
	float3 dir;
	float pad;
	float4 ambient;
	float4 diffuse;
	float4 specular;
};

cbuffer cbPerObject: register(b0)
{
	matrix WVP;
	matrix World;

	bool hasTexture;
};

cbuffer cbPerFrame : register(b1)
{
	Light light;
};

struct Material
{
	float3 AmbientMaterial;
	float pad1;
	float3 DiffuseMaterial;
	float pad2;
	float3 SpecularMaterial;
	float SpecularPower;
};

cbuffer Material : register(b2)
{
	Material material;
};

cbuffer cbCamera:register(b3)
{
	float3 CameraPos;
	float pad4;
};

Texture2D ObjTexture : register(t0);
SamplerState ObjSamplerState : register(s0);

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 worldPos : POSITION;
	float2 TexCoord : TEXCOORD;
	float3 normal : NORMAL;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	VS_OUTPUT output;

	output.Pos = mul(inPos, WVP);

	output.worldPos = mul(inPos, World);

	output.normal = mul(normal, World);

	output.normal = normalize(output.normal);

	output.TexCoord = inTexCoord;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float DiffuseFactor;
	float SpecularFactor;
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 color;

	//��ʼ��Ambient,Diffuse,Specular  
	Ambient = float4(0.0f, 0.0f, 0.0f, 1.0f);
	Diffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
	Specular = float4(0.0f, 0.0f, 0.0f, 1.0f);
	color = float4(0.0f, 0.0f, 0.0f, 1.0f);

	//��������ɫ
	Ambient = light.ambient * float4(material.AmbientMaterial, 1.0f);

	float3 NormalLightDir = light.dir;

	//�������������  
	float3 InvLightDir = -NormalLightDir;
	DiffuseFactor = saturate(dot(InvLightDir, input.normal));

	//�����������ɫ  
	if (DiffuseFactor>0)
	{
		Diffuse = DiffuseFactor * light.diffuse*float4(material.DiffuseMaterial, 1.0f);
		
	}

	if (hasTexture == true)
	{
		Diffuse = ObjTexture.Sample(ObjSamplerState, input.TexCoord);
		clip(Diffuse.w < 0.1f ? -1 : 1);
	}
		

	/*������������������ɫ*/
	float3 ReflectLightDir = reflect(light.dir, input.normal);

	//��񻯷���ⷽ��  
	ReflectLightDir = normalize(ReflectLightDir);

	//�������㵽���֮�������  
	float3 PixelToCameraPosVector = CameraPos - input.worldPos;

	//���PixelToCameraPosVector  
	PixelToCameraPosVector = normalize(PixelToCameraPosVector);

	//�����������  
	SpecularFactor = pow(saturate(dot(PixelToCameraPosVector, ReflectLightDir)), material.SpecularPower);

	//����������ɫ
	if (SpecularFactor > 0)
	{
		Specular = SpecularFactor * light.diffuse*float4(material.SpecularMaterial, 1.0f);
	}

	color = saturate(Ambient + Diffuse + Specular);

	return color;
}