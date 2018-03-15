#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>


int main()
{
	IplImage *InImage, *MidImage,*OutImage,*OutImage1,*OutImage2,*OutImage3;
	int label=1,**Imagelabel,x,y,max,flag=0,i,j,*Imagelabelnum;
	char InputFileName[50];


	printf("Enter file name.\n");
	scanf("%s", &InputFileName);

	InImage = cvLoadImage(InputFileName, CV_LOAD_IMAGE_COLOR);
	if(!InImage)
	{
		printf("Error: Couldn't open the image file.\n");
		system("pause");
		return -1;
	}


	
	MidImage = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	OutImage = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	OutImage1 = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	OutImage2 = cvCreateImage(cvSize(InImage->width, InImage->height), 8, 3);
	OutImage3 = cvCreateImage(cvSize(2*(InImage->width), 2*(InImage->height)), 8, 3);

	cvNamedWindow("initial", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("二質化", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("標記-4", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("同化-4", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("放大-4", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("同化-8", CV_WINDOW_AUTOSIZE);


	y=InImage -> width;
	x=InImage -> height;
	
	Imagelabel = (int**)malloc(x*sizeof(int *)); 
	for (i=0;i<x;i++)
	{
		Imagelabel[i]=(int*)malloc(y*sizeof(int));
	}
	for (i=0;i<x;i++)
	{
		for(j=0;j<y;j++)
		{
			Imagelabel[i][j]=0;
		}

	}

	printf("%d  %d\n",x,y);
	//2質化


		cvShowImage("initial", InImage);

		for(i=0; i<InImage->height; i++){
			for(j=0; j<InImage->width; j++){
				CvScalar x = cvGet2D(InImage, i, j);
			    int y = 0.299*x.val[2]+0.587*x.val[1]+0.114*x.val[0];
				cvSet2D(OutImage, i, j, CV_RGB(y,y,y) );
				if(y>125)
					cvSet2D(MidImage, i, j, CV_RGB(255, 255, 255));
				else
					cvSet2D(MidImage, i, j, CV_RGB(0, 0, 0));

			}
		}
		
	
	
	
	
		//標記 4
		cvShowImage("二質化", MidImage);
		
		for(i=0; i<MidImage->height; i++)
		{
			for(j=0; j<MidImage->width; j++)
			{
				CvScalar x = cvGet2D(MidImage, i, j);
				if(x.val[2]==255)
				{
					if (label==1)
					{
						Imagelabel[i][j]=label;
						label=label+1;
						
					}
					else if (i==0)
					{
						if (Imagelabel[i][j-1]>0)
						{
							Imagelabel[i][j]=Imagelabel[i][j-1];
						}
						else
						{
							Imagelabel[i][j]=label;
							label=label+1;
						}
						
					}
					else if (j==0)
					{
						if (Imagelabel[i-1][j]>0)
						{
							Imagelabel[i][j]=Imagelabel[i-1][j];
						}
						else
						{
							Imagelabel[i][j]==label;
							label=label+1;
						}
						
					}
					else 
					{
						if (Imagelabel[i][j-1]!=0 && Imagelabel[i-1][j]!=0)
						{
							if (Imagelabel[i][j-1]>Imagelabel[i-1][j])
							{
								Imagelabel[i][j]=Imagelabel[i][j-1];
							}
							else
							{
								Imagelabel[i][j]=Imagelabel[i-1][j];
							}
						}
						else if (Imagelabel[i][j-1]!=0 )
						{
							Imagelabel[i][j]=Imagelabel[i][j-1];
						}
						else if (Imagelabel[i-1][j]!=0) 
						{
							Imagelabel[i][j]=Imagelabel[i-1][j];
						}
						else
						{
							label=label+1;
							Imagelabel[i][j]=label;
						}
						
					}
					if (Imagelabel[i][j]%7==0)
						{
							cvSet2D(OutImage, i, j, CV_RGB((128+(Imagelabel[i][j]%64)*2), 0, 0));
						}
						else if (Imagelabel[i][j]%7==1)
						{	
							cvSet2D(OutImage, i, j, CV_RGB(0, (128+(Imagelabel[i][j]%64)*2), 0));	
						}
						else if (Imagelabel[i][j]%7==2)
						{
							cvSet2D(OutImage, i, j, CV_RGB(0, 0, (128+(Imagelabel[i][j]%64)*2)));
						}
						else if (Imagelabel[i][j]%7==3)
						{
							cvSet2D(OutImage, i, j, CV_RGB((128+(Imagelabel[i][j]%64)*2), (128+(Imagelabel[i][j]%64)*2), 0));
						}
						else if (Imagelabel[i][j]%7==4)
						{
							cvSet2D(OutImage, i, j, CV_RGB((128+(Imagelabel[i][j]%64)*2), 0, (128+(Imagelabel[i][j]%64)*2)));
						}
						else if (Imagelabel[i][j]%7==5)
						{
							cvSet2D(OutImage, i, j, CV_RGB(0, (128+(Imagelabel[i][j]%64)*2), (128+(Imagelabel[i][j]%64)*2)));
						}
						else 
						{
							cvSet2D(OutImage, i, j, CV_RGB((128+(Imagelabel[i][j]%64)*2), (128+(Imagelabel[i][j]%64)*2), (128+(Imagelabel[i][j]%64)*2)));
						}
				}
				else
				{
					cvSet2D(OutImage, i, j, CV_RGB(0, 0, 0));
				}
			}
		}

		
	Imagelabelnum=(int *)malloc(label*sizeof(int));
	
	for (i=0;i<label;i++)
	{
		Imagelabelnum[i]=i;
	}
		cvShowImage("標記-4", OutImage);
	//標籤同化
	
	while (flag==0 && OutImage != NULL)
	{
		flag=1;
		for(i=0; i<InImage->height; i++)
		{
			for(j=0; j<InImage->width; j++)
			{
				if(Imagelabel[i][j]>0)
				{
					max=Imagelabel[i][j];
					if (i>0)
					{
						if(Imagelabel[i-1][j]>0 )
						{
							if (Imagelabel[i-1][j]>max)
							{
								max=Imagelabel[i-1][j];
								flag=0;
							}
						}
					}
					if (j>0)
					{
						if(Imagelabel[i][j-1]>0)
						{
							if (Imagelabel[i][j-1]>max)
							{
								max=Imagelabel[i][j-1];
								flag=0;
							}
						}
					}
					if (i<((InImage->height)-1))
					{
						if(Imagelabel[i+1][j]>0)
						{
							if (Imagelabel[i+1][j]>max)
							{
								flag=0;
								max=Imagelabel[i+1][j];
							}
						}
					}
					if (j<((InImage->width)-1))
					{
						if(Imagelabel[i][j+1]>0)
						{
							if (Imagelabel[i][j+1]>max)
							{
								flag=0;
								max=Imagelabel[i][j+1];
							}
						}
					}
					Imagelabel[i][j]=max;
						if (Imagelabel[i][j]%7==0)
						{
							cvSet2D(OutImage1, i, j, CV_RGB((128+(Imagelabel[i][j]%64)*2), 0, 0));
						}
						else if (Imagelabel[i][j]%7==1)
						{	
							cvSet2D(OutImage1, i, j, CV_RGB(0, (128+(Imagelabel[i][j]%64)*2), 0));	
						}
						else if (Imagelabel[i][j]%7==2)
						{
							cvSet2D(OutImage1, i, j, CV_RGB(0, 0, (128+(Imagelabel[i][j]%64)*2)));
						}
						else if (Imagelabel[i][j]%7==3)
						{
							cvSet2D(OutImage1, i, j, CV_RGB((128+(Imagelabel[i][j]%64)*2), (128+(Imagelabel[i][j]%64)*2), 0));
						}
						else if (Imagelabel[i][j]%7==4)
						{
							cvSet2D(OutImage1, i, j, CV_RGB((128+(Imagelabel[i][j]%64)*2), 0, (128+(Imagelabel[i][j]%64)*2)));
						}
						else if (Imagelabel[i][j]%7==5)
						{
							cvSet2D(OutImage1, i, j, CV_RGB(0, (128+(Imagelabel[i][j]%64)*2), (128+(Imagelabel[i][j]%64)*2)));
						}
						else 
						{
							cvSet2D(OutImage1, i, j, CV_RGB((128+(Imagelabel[i][j]%64)*2), (128+(Imagelabel[i][j]%64)*2), (128+(Imagelabel[i][j]%64)*2)));
						}
				}
				else
				{
					cvSet2D(OutImage1, i, j, CV_RGB(0, 0, 0));
				}
			}
		}
	}
	
		cvShowImage("同化-4", OutImage1);

	
	//放大
		for(i=0; i<OutImage1->height; i++)
		{
			for(j=0; j<OutImage1->width; j++)
			{
				CvScalar x = cvGet2D(OutImage1, i, j);
				cvSet2D(OutImage3, 2*i, 2*j, CV_RGB(x.val[2], x.val[1], x.val[0]));
				cvSet2D(OutImage3, 2*i+1, 2*j, CV_RGB(x.val[2], x.val[1], x.val[0]));
				cvSet2D(OutImage3, 2*i, 2*j+1, CV_RGB(x.val[2], x.val[1], x.val[0]));
				cvSet2D(OutImage3, 2*i+1, 2*j+1, CV_RGB(x.val[2], x.val[1], x.val[0]));
			}
		}
	
		cvShowImage("放大-4", OutImage3);
	flag=0;

	//標記8
	while (flag==0 && OutImage1 != NULL)
	{
		flag=1;
		for(i=0; i<InImage->height; i++)
		{
			for(j=0; j<InImage->width; j++)
			{
				if(Imagelabel[i][j]>0)
				{
					max=Imagelabel[i][j];
					if (i>0)
					{
						if(Imagelabel[i-1][j]>0 )
						{
							if (Imagelabel[i-1][j]>max)
							{
								max=Imagelabel[i-1][j];
								flag=0;
							}
						}
					}
					if (j>0)
					{
						if(Imagelabel[i][j-1]>0)
						{
							if (Imagelabel[i][j-1]>max)
							{
								max=Imagelabel[i][j-1];
								flag=0;
							}
						}
					}
					if (i<((InImage->height)-1))
					{
						if(Imagelabel[i+1][j]>0)
						{
							if (Imagelabel[i+1][j]>max)
							{
								flag=0;
								max=Imagelabel[i+1][j];
							}
						}
					}
					if (j<((InImage->width)-1))
					{
						if(Imagelabel[i][j+1]>0)
						{
							if (Imagelabel[i][j+1]>max)
							{
								flag=0;
								max=Imagelabel[i][j+1];
							}
						}
					}
					if (i>0&&j>0)
					{
						if(Imagelabel[i-1][j-1]>0 )
						{
							if (Imagelabel[i-1][j-1]>max)
							{
								max=Imagelabel[i-1][j-1];
								flag=0;
							}
						}
					}
					if (i>0&&j<((InImage->width)-1))
					{
						if(Imagelabel[i-1][j+1]>0 )
						{
							if (Imagelabel[i-1][j+1]>max)
							{
								max=Imagelabel[i-1][j+1];
								flag=0;
							}
						}
					}
					if (i<((InImage->height)-1) && j>0)
					{
						if(Imagelabel[i+1][j-1]>0)
						{
							if (Imagelabel[i+1][j-1]>max)
							{
								max=Imagelabel[i+1][j-1];
								flag=0;
							}
						}
					}
					if (i<((InImage->height)-1)&&j<((InImage->width)-1))
					{
						if(Imagelabel[i+1][j+1]>0)
						{
							if (Imagelabel[i+1][j+1]>max)
							{
								flag=0;
								max=Imagelabel[i+1][j+1];
							}
						}
					}
					Imagelabel[i][j]=max;
						if (Imagelabel[i][j]%7==0)
						{
							cvSet2D(OutImage2, i, j, CV_RGB((128+(Imagelabel[i][j]%64)*2), 0, 0));
						}
						else if (Imagelabel[i][j]%7==1)
						{	
							cvSet2D(OutImage2, i, j, CV_RGB(0, (128+(Imagelabel[i][j]%64)*2), 0));	
						}
						else if (Imagelabel[i][j]%7==2)
						{
							cvSet2D(OutImage2, i, j, CV_RGB(0, 0, (128+(Imagelabel[i][j]%64)*2)));
						}
						else if (Imagelabel[i][j]%7==3)
						{
							cvSet2D(OutImage2, i, j, CV_RGB((128+(Imagelabel[i][j]%64)*2), (128+(Imagelabel[i][j]%64)*2), 0));
						}
						else if (Imagelabel[i][j]%7==4)
						{
							cvSet2D(OutImage2, i, j, CV_RGB((128+(Imagelabel[i][j]%64)*2), 0, (128+(Imagelabel[i][j]%64)*2)));
						}
						else if (Imagelabel[i][j]%7==5)
						{
							cvSet2D(OutImage2, i, j, CV_RGB(0, (128+(Imagelabel[i][j]%64)*2), (128+(Imagelabel[i][j]%64)*2)));
						}
						else 
						{
							cvSet2D(OutImage2, i, j, CV_RGB((128+(Imagelabel[i][j]%64)*2), (128+(Imagelabel[i][j]%64)*2), (128+(Imagelabel[i][j]%64)*2)));
						}
				}
				else
				{
					cvSet2D(OutImage2, i, j, CV_RGB(0, 0, 0));
				}
			}
		}
	}
	
	while(OutImage != NULL)
	{
		cvShowImage("同化-8", OutImage2);
		char c;
		c = cvWaitKey(1);
		if(c == 'q') break;
	}
	cvReleaseImage(&InImage);
	cvReleaseImage(&MidImage);
	cvReleaseImage(&OutImage);
	cvReleaseImage(&OutImage1);
	cvReleaseImage(&OutImage2);
	cvReleaseImage(&OutImage3);
	cvDestroyWindow("initial");    // 關閉視窗
	cvDestroyWindow("二質化");
	cvDestroyWindow("標記-4");
	cvDestroyWindow("同化-4");
	cvDestroyWindow("放大-4");
	cvDestroyWindow("同化-8");
	
	for (i=0;i<x;i++)
	{
		free(Imagelabel[i]);
	}

	free(Imagelabel);
	
	return 0;
}