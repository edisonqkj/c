#include <stdio.h>
#include <malloc.h>

typedef struct Image{
	int Width;
	int Height;
	unsigned char* Red;
	unsigned char* Green;
	unsigned char* Blue;
	unsigned char* Alpha;
}Image,*pImage;

/// Print Image Data:
void Print(unsigned char* image, int width, int height)
{
	/// print image
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++)
			printf("%u ",image[i*width+j]);
		printf("\n");
	}
}
/// Write Ascii File:
/// Write File Format:
/// +each element splitting with comma
void WriteAscii(char* file,unsigned char* image, int width, int height)
{
	FILE* fp=fopen(file,"w");
	for(int i=0;i<height;i++)
	{
		fprintf(fp,"%u",image[i*width+0]);
		for(int j=1;j<width;j++)
		{
			fprintf(fp,",%u",image[i*width+j]);
		}
		fprintf(fp,"\n");
	}
	fclose(fp);
}
/// Read Binary File:
/// Read File format:
/// +first two int elements:	width, height
/// +left:			image data (4 unsigned char (RGBA))
/// Return:
/// +pImage: 			pointer of Image struct (4 Bands)
void ReadBinary(char* file,pImage image)
{
	FILE* fp=fopen(file,"rb");
	/// read first two int values: width and height
	fread(&(image->Width),sizeof(int),1,fp);
	fread(&(image->Height),sizeof(int),1,fp);

	/// malloc memory for each band of image data
	image->Red=(unsigned char*)malloc((image->Width)*(image->Height)*sizeof(unsigned char));
	image->Green=(unsigned char*)malloc((image->Width)*(image->Height)*sizeof(unsigned char));
	image->Blue=(unsigned char*)malloc((image->Width)*(image->Height)*sizeof(unsigned char));
	image->Alpha=(unsigned char*)malloc((image->Width)*(image->Height)*sizeof(unsigned char));

	int IsRGBAPair=1;
	/// Band data is (RGBA) pair
	if(IsRGBAPair)
	{
		/// read 4 bands data
		unsigned char* buf=(unsigned char*)malloc(4*(image->Width)*(image->Height)*sizeof(unsigned char));
		fread(buf,sizeof(unsigned char),4*(image->Width)*(image->Height),fp);

		/// store into Red, Green, Blue, Alpha bands
		for(int i=0;i<(image->Width)*(image->Height);i+=4)
		{
			image->Red[i]=buf[i];
			image->Green[i]=buf[i+1];
			image->Blue[i]=buf[i+2];
			image->Alpha[i]=buf[i+3];
		}
		free(buf);
		buf=NULL;
	}
	else/// each of RGBA is seperately stored
	{
		fread(image->Red,sizeof(unsigned char),(image->Width)*(image->Height),fp);
		fread(image->Green,sizeof(unsigned char),(image->Width)*(image->Height),fp);
		fread(image->Blue,sizeof(unsigned char),(image->Width)*(image->Height),fp);
		fread(image->Alpha,sizeof(unsigned char),(image->Width)*(image->Height),fp);
	}
	fclose(fp);
}
/// Run Encoding Image:
/// Save File format:
/// +first two int elements:	width, height
/// +left:			(value,count) pairs
void RunEncoding(char* file,unsigned char* image, int width, int height)
{
	FILE* fp=fopen(file,"wb");
	fwrite(&width,sizeof(int),1,fp);
	fwrite(&height,sizeof(int),1,fp);

	unsigned char value;
	int count;
	/// encode each row
	for(int i=0;i<height;i++)
	{
		printf("%d: ",i);
		value=image[i*width+0];
		count=1;
		/// check each element in the row
		for(int j=1;j<width;j++)
		{
			/// the same value
			if(image[i*width+j]==value)
			{
				count++;/// sum the same value
				/// if the whole row is checked
				if(j==width-1)
				{
					printf("(%u,%d) ",value,count);
					fwrite(&value,sizeof(unsigned char),1,fp);
					fwrite(&count,sizeof(int),1,fp);
				}
			}
			else/// value changes
			{
				/// print current value and count
				printf("(%u,%d) ",value,count);
				fwrite(&value,sizeof(unsigned char),1,fp);
				fwrite(&count,sizeof(int),1,fp);

				/// new value
				value=image[i*width+j];
				count=1;
			}
		}
		printf("\n");
	}
	fclose(fp);
}

/// Run Decoding Image:
/// Read File format:
/// +first two elements:	width, height
/// +left:			(value,count) pairs
/// Return:
/// +image: 			pointer of image array (1 Band)
/// +widht: 			width of image data
/// +height:			height of image data
void RunDecoding(char* file,unsigned char** image,int* width, int* height)
{
	FILE* fp=fopen(file,"rb");
	/// read first two int values: width and height
	fread(width,sizeof(int),1,fp);
	fread(height,sizeof(int),1,fp);

	/// malloc memory for 1 band of image data
	if(*image!=NULL)
		free(*image);
	*image=(unsigned char*)malloc((*width)*(*height)*sizeof(unsigned char));

	/// read (value,count) one by one
	/// current: record where the new value starts in image
	unsigned char value;
	int count,current=0;

	/// does not arrive at the end of file 
	while(!feof(fp))
	{
		fread(&value,sizeof(unsigned char),1,fp);/// read value
		fread(&count,sizeof(int),1,fp);/// read count

		/// store data into the image array
		for(int i=current;i<current+count;i++)
		{
			(*image)[i]=value;
		}
		current+=count;
	}
	fclose(fp);
}

void main()
{
	pImage image=(pImage)malloc(sizeof(Image));
	/// read binary image data
	ReadBinary("image.dat",image);

	/// print binary image data
	printf("Binary Image:\n");
	//Print(image->Red,image->Width,image->Height);

	/// write ascii image data
	WriteAscii("asc.txt",image->Red,image->Width,image->Height);

	/// run encoding and save
	printf("RunEncoding:\n");
	RunEncoding("runcode",image->Red,image->Width,image->Height);

	unsigned char* nbuf=NULL;
	int nw,nh;

	/// run decoding
	RunDecoding("runcode",&nbuf,&nw,&nh);

	/// print decoded image data
	printf("Decoded Image:\n");
	//Print(nbuf,nw,nh);
	getchar();
}

