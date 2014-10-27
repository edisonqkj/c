#include <stdio.h>
#include <malloc.h>

/// Print Image Data:
void Print(int* image, int width, int height)
{
	/// print image
	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++)
			printf("%d ",image[i*width+j]);
		printf("\n");
	}
}

/// Read Binary File:
/// Read File format:
/// +first two int elements:	width, height
/// +left:			image data (int elements)
/// Return:
/// +image: 			pointer of image array
/// +widht: 			width of image data
/// +height:			height of image data
void ReadBinary(char* file,int** image,int* width,int* height)
{
	FILE* fp=fopen(file,"rb");
	/// read first two int values: width and height
	fread(width,sizeof(int),1,fp);
	fread(height,sizeof(int),1,fp);

	/// malloc memory for image data
	if(*image!=NULL)
		free(*image);
	*image=(int*)malloc((*width)*(*height)*sizeof(int));

	/// read image data
	fread(*image,sizeof(int),(*width)*(*height),fp);

	fclose(fp);
}
/// Run Encoding Image:
/// Save File format:
/// +first two int elements:	width, height
/// +left:			(value,count) pairs
void RunEncoding(char* file,int* image, int width, int height)
{
	FILE* fp=fopen(file,"wb");
	fwrite(&width,sizeof(int),1,fp);
	fwrite(&height,sizeof(int),1,fp);

	int value,count;
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
					printf("(%d,%d) ",value,count);
					fwrite(&value,sizeof(int),1,fp);
					fwrite(&count,sizeof(int),1,fp);
				}
			}
			else/// value changes
			{
				/// print current value and count
				printf("(%d,%d) ",value,count);
				fwrite(&value,sizeof(int),1,fp);
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
/// +image: 			pointer of image array
/// +widht: 			width of image data
/// +height:			height of image data
void RunDecoding(char* file,int** image,int* width, int* height)
{
	FILE* fp=fopen(file,"rb");
	/// read first two int values: width and height
	fread(width,sizeof(int),1,fp);
	fread(height,sizeof(int),1,fp);

	/// malloc memory for image data
	if(*image!=NULL)
		free(*image);
	*image=(int*)malloc((*width)*(*height)*sizeof(int));

	/// read (value,count) one by one
	/// current: record where the new value starts in image
	int value,count,current=0;

	/// does not arrive at the end of file 
	while(!feof(fp))
	{
		fread(&value,sizeof(int),1,fp);/// read value
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
	int* buf=NULL;
	int w,h;
	/// read binary image data
	ReadBinary("asc_image",&buf,&w,&h);

	/// print binary image data
	printf("Binary Image:\n");
	Print(buf,w,h);

	/// run encoding and save
	printf("RunEncoding:\n");
	RunEncoding("runcode",buf,w,h);

	int* nbuf=NULL;
	int nw,nh;

	/// run decoding
	RunDecoding("runcode",&nbuf,&nw,&nh);

	/// print decoded image data
	printf("Decoded Image:\n");
	Print(nbuf,nw,nh);
}

