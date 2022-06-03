#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	int i, j, k, L_int, check_saturation;
	double L, h;
	
	L = 0.5;
	L_int = 16;	
	h = L/L_int;

	double V[L_int+1][L_int+1][L_int+1], prevV[L_int+1][L_int+1][L_int+1];

	for(i=0; i<=L_int; i++)
	{
		for(j=0; j<=L_int; j++)
			V[i][j][0] = 0;
	}	
	
	for(j=0; j<=L_int; j++)
	{
		for(k=0; k<=L_int; k++)
			V[L_int][j][k] = 0;
	}

	for(j=0; j<=L_int; j++)
	{
		for(k=0; k<=L_int; k++)
			V[0][j][k] = 0;
	}
	
	for(i=0; i<=L_int; i++)
	{
		for(k=0; k<=L_int; k++)
			V[i][0][k] = 0;
	}

	for(i=0; i<=L_int; i++)
	{
		for(k=0; k<=L_int; k++)
			V[i][L_int][k] = 0;
	}

	for(i=0; i<=L_int; i++)
	{
		for(j=0; j<=L_int; j++)
			V[i][j][L_int] = 0;
	}

	V[L_int/2][L_int/2][L_int/2] = 1;

	for(i=1; i<L_int; i++)
	{
		for(j=1; j<L_int; j++)
		{
			for(k=1; k<L_int; k++)
				V[i][j][k] = 1;
		}
	}

	for(i=0; i<=L_int; i++)
	{
		for(j=0; j<=L_int; j++)
		{
			for(k=0; k<=L_int; k++)
				prevV[i][j][k] = V[i][j][k];
		}
	}	

	while(1)
	{
		for(i=1; i<L_int; i++)
		{
			for(j=1; j<L_int; j++)
			{
				for(k=1; k<L_int; k++)
				{
					if((i == 3*L_int/8 && j == L_int/2 && k == L_int/2 )||(i == 5*L_int/8 && j == L_int/2 && k == L_int/2))
						continue;

					V[i][j][k] = (V[i+1][j][k]+V[i-1][j][k]+V[i][j+1][k]+V[i][j-1][k]+V[i][j][k+1]+V[i][j][k-1])/6.0;
				 	

				}
			}
		}


/*	 while(1)
	 {   
		 for(i=1; i<L_int; i++)			
		 {								    
      			 for(j=1; j<L_int; j++)  
	      		 {
	                          for(k=1; k<L_int; k++)
				  { 
				     	  if(i == 5*L_int/8 && j == L_int/2 && k == L_int/2)
					  	continue;
					  
					  V[i][j][k] = (V[i+1][j][k]+V[i-1][j][k]+V[i][j+1][k]+V[i][j-1][k]+V[i][j][k+1]+V[i][j][k-1])/6.0;											
				 }
			}
		}
	}
*/
		check_saturation = 0;
		for(i=1; i<L_int; i++)
		{
			for(j=1; j<L_int; j++)
			{
				for(k=1; k<L_int; k++)
				{
					if(i == L_int/2 && j == L_int/2 && k == L_int/2)
						continue;

					if(fabs(V[i][j][k]-prevV[i][j][k]) > 1e-8)
						check_saturation = 1;
				}
			}
		}

		if(check_saturation == 0)
			break;
	
		for(i=1; i<L_int; i++)
		{
			for(j=1; j<L_int; j++)
			{
				for(k=1; k<L_int; k++)
					prevV[i][j][k] = V[i][j][k];
			}
		}
	}



	double Ex, Ey, Ez;
	FILE* fp;
	fp = fopen("3d_laplace_twocharges_Efield", "w");

	for(i=1; i<L_int; i++)
	{
		for(j=1; j<L_int; j++)
		{
			for(k=1; k<L_int; k++)
			{
				Ex = -(V[i+1][j][k]-V[i-1][j][k])/(2.0*h);
				Ey = -(V[i][j+1][k]-V[i][j-1][k])/(2.0*h);
				Ez = -(V[i][j][k+1]-V[i][j][k-1])/(2.0*h);
				fprintf(fp, "%f %f %f %f %f %f\n", i*h, j*h, k*h, Ex, Ey, Ez);
			}
		}
	}
	
	for(i=1; i<L_int; i++)
	{
		for(j=1; j<L_int; j++)
		{
			for(k=1; k<L_int; k++)
			{	Ex = -(V[i+1][j][k]-V[i-1][j][k])/(2.0*h);
			        Ey = -(V[i][j+1][k]-V[i][j-1][k])/(2.0*h);
				Ez = -(V[i][j][k+1]-V[i][j][k-1])/(2.0*h);
				printf("(%f, %f, %f, %f, %f, %f)\n", i*h, j*h, k*h, Ex, Ey, Ez);
			}
		}

	}
	
	

//	double laplace_check;

//	for(i=1; i<L_int; i++)
//	{
//		for(j=1; j<L_int; j++)
//		{
//			for(k=1; k<L_int; k++)
//			{
//				laplace_check = 6*V[i][j][k] - V[i+1][j][k] - V[i-1][j][k] - V[i][j+1][k] - V[i][j-1][k] - V[i][j][k+1] - V[i][j][k-1];
//				printf("%d %d %d %f\n", i, j, k, laplace_check);
//			}
//		}
//	}
	
	return 0;
}
