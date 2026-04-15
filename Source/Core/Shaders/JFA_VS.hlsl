#include "Includes/JFA.hlsli"

JFA_VS_Out main(JFA_VS_In input)
{
    JFA_VS_Out output;
    output.Rectangle = input.Rectangle;
	return output;
}