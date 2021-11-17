#include <stdio.h>
#include <time.h>
#include <omp.h>

int main(int argc, char **argv)
{
	clock_t start, stop;

	start = clock();

	char* filename = argv[1];

	printf("File: %s\n", filename);

	FILE *fIn = fopen(filename,"r");
	FILE *fOut = fopen("bw_parallel.bmp","w+");

	int i,j,y;
	unsigned char byte[54];
	
	if(fIn==NULL)
	{											
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++) //read the 54 byte header from fIn
	{									
		byte[i] = getc(fIn);							
	}

	fwrite(byte, sizeof(unsigned char), 54, fOut); //write the header back

	// extract image height, width and bitDepth from imageHeader 
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int size = height*width; //calculate image size

	unsigned char buffer[size][3]; //to store the image data
	unsigned char buffer_bw[size]; //to store the b&w image data
	
	for(i=0;i<size;i++){
        
		buffer[i][2]=getc(fIn); //blue
		buffer[i][1]=getc(fIn); //green
		buffer[i][0]=getc(fIn); //red
    }
		
    #pragma omp parallel for
	for(i=0;i<size;i++){

		y = (buffer[i][0]*0.3) + (buffer[i][1]*0.59) + (buffer[i][2]*0.11); //conversion formula of rgb to gray
        buffer_bw[i] =  y;
    }

	for(i=0;i<size;i++){

        y = buffer[i][0];

		putc(y,fOut);
		putc(y,fOut);
		putc(y,fOut);
    }
	
	fclose(fOut);
	fclose(fIn);

	stop = clock();
	printf("\nCLOCKS_PER_SEC = %ld\n",stop-start); 
	printf("%lf ms\n",((double)(stop-start) * 1000.0)/CLOCKS_PER_SEC );
	return 0;
}
