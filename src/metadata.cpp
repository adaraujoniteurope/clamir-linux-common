#include "metadata.h"
#include "math.h"

/*
 * Funcion de cáclculo de ancho, empleada por varios estados del automata de control
 */
double metadata_get_width(metadata_t* metadata)
{
	double W = 0;
	double X, Y;
	double u20, u11, u02;

	// Procesos de calculo
	if (metadata->M00 == 0)
		metadata->M00 = 1;
	X = metadata->M10 / metadata->M00;
	Y = metadata->M01 / metadata->M00;

	u20 = (metadata->M20 / metadata->M00) - (X * X);
	u11 = (metadata->M11 / metadata->M00) - (X * Y);
	u02 = (metadata->M02 / metadata->M00) - (Y * Y);

	W = sqrt(8 * (u20 + u02 - sqrt((4 * u11 * u11) + ((u20 - u02) * (u20 - u02))))); // Aqui se ha calculado el ancho
	return W;
}