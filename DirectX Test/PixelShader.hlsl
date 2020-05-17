

float4 main( uint id : SV_PrimitiveID) : SV_Target
{ float4 colors[6] =
{
	{1.0f, 0.0f, 0.0f, 1.0f},
	{0.0f, 1.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 1.0f, 1.0f},
	{1.0f, 1.0f, 0.0f, 1.0f},
	{0.0f, 1.0f, 1.0f, 1.0f},
	{1.0f, 0.0f, 1.0f, 1.0f}
};
	return colors[(id/2)%6];
}