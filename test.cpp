#include <iostream>
#include "opencv2/opencv.hpp"
#include<string>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>

using namespace std;
using namespace cv;

void onMouse(int event,int x,int y,int flags, void* userdata);
void face_register(void);
void keyuser(int fps,int cw,int ch);


FILE*  mplayer_init(void);
void mplayer_play(FILE* fp,const char* music_path);
void mplayer_pause(FILE* fp);
void mplayer_continue(FILE* fp);
void mplayer_stop(FILE* fp);
void mplayer_set(FILE* fp,int volume);
void mplayer_exit(FILE* fp);




string face_recongnize(Mat face_img,Mat face_box);

Mat image(640,480,CV_8UC3,Scalar(255,255,255));
Mat img,watermark,faces,lx;
int lx_x=100,lx_y=100;

Mat chensiyu_feature[100];
char names[100][100]; //人脸名字
int face_cnt = 0;         //人脸总数


Ptr<FaceDetectorYN> fd = FaceDetectorYN::create("/home/csy2022/day4/code1/models/yunet.onnx","",Size(320,320));
Ptr<FaceRecognizerSF> fr = FaceRecognizerSF::create("/home/csy2022/day4/code1/models/face_recognizer_fast.onnx","");


int main()
{
//	system("mplayer 没有什么大不了的.mp3 &"); //&返回当前程序
	FILE* fp = mplayer_init();
	
	VideoCapture camera(0);  // 定义一个 VideoCapture 类的对象 camera，用于访问摄像头（第 0 个）
	int r = 35, flag = 0;
	int fps = camera.get(CAP_PROP_FPS);
	int cw = camera.get(CAP_PROP_FRAME_WIDTH);
	int ch = camera.get(CAP_PROP_FRAME_HEIGHT);
	int chensiyu_close = 0;
	int only = 0;
	int t = time(NULL);

	string name;
        int i, j,temp;

	int face_x,face_y,face_w,face_h,face_rx,face_ry,face_lx,face_ly,p;

      	srand(time(NULL));

        cerr << rand() << endl;
	face_register();
        while(1)
	{
		if(!camera.read(img))  // 通过摄像头捕获一帧图像并存放到 img 中
		{
			cerr << "访问摄像头失败！" << endl;
			return 1;
		}

                flip(img,img,1);
		keyuser(fps,cw,ch);
                fd -> setInputSize(img.size());   //设置输入尺寸
		fd -> detect(img,faces);         //检测出img中包含的所有人脸信息
		//faces 中每一行代表一个人脸信息，15列分别是人脸区域左上角坐标，人脸区域宽度和高度，右眼坐标，左眼坐标，鼻子坐标，左嘴角，右嘴角，置信度
                for(p = 0;p<faces.rows;p++)             //表示人脸的个数
		{
			face_x = faces.at<float>(p,0);
			face_y = faces.at<float>(p,1);
			face_w = faces.at<float>(p,2);
			face_h = faces.at<float>(p,3);
			/*face_rx = faces.at<float>(p,4);
			face_ry = faces.at<float>(p,5);
			face_lx = faces.at<float>(p,6);
			face_ly = faces.at<float>(p,7);

			circle(img, Point(face_rx,face_ry), r, Scalar(0, 0, 255), 2);
			circle(img, Point(face_lx,face_ly), r, Scalar(0, 0, 255), 2);*/
			rectangle(img,Rect(face_x,face_y,face_w,face_h),Scalar(200,150,0),2);
			putText(img,name,Point(face_x,face_y - 5),FONT_HERSHEY_SIMPLEX,1.0,Scalar(150,200,0),2);
			name = face_recongnize(img,faces.row(p));
			cout << name << endl;
			cout << names[face_cnt] << endl;
			if(name == "bobo")
			{	
				chensiyu_close = 1;
			}
			else
			{	
				chensiyu_close = 0;
			}
			cout << chensiyu_close << endl;
		}
		//lx = imread("/home/csy2022/day4/code1/OIP-C.jpg");
		//Mat roi = img(Rect(100,100,200,200));     //后两个参数代表图像的宽度和高
		//lx.copyTo(roi);
		//imshow("实时人脸识别系统",img);
		if(!chensiyu_close)
		{
			if(time(NULL) - t>10)
			{
				if(only == 0)
				{
					mplayer_play(fp,"没有什么大不了.mp3");
					only = 1;
				}

			}
		}
		else
		{
			t = time(NULL);
		}
                imshow("实时人脸识别系统", img);  // 将 img 显示出来
	}
	mplayer_exit(fp);
	return 0;
}



//注册人脸
void face_register(void)
{
	Mat faces,aligned_img,face_feature;
	vector<string> path;
	int i;
        const char* name;
	const char* faces_dir = "/home/csy2022/day4/code1/人像信息/";                          //字符串常量
//获取文件夹中所有的文件名，并把它们放在path数组中
	glob(faces_dir,path);

		for(auto p : path)
		{		
			img = imread(p);
			if(img.cols > 1024)
			{
				resize(img,img,Size(),1024.0 / img.cols,1024.0 / img.cols);
			}
			if(img.cols < 80)
			{
				cout << "图像过小无法识别" << p << endl;
			}
			fd->setInputSize(img.size());
			fd->detect(img,faces);  // 检测出人脸所在区域
			if(faces.rows  < 1)
			{
				cerr << "录入不到人脸或者检测到多个人脸" << endl;
				continue;
			}
			fr ->alignCrop(img,faces.row(0),aligned_img); //对人像进行对齐和裁减处理，并且将结果存放到aligned_img中
			fr ->feature(aligned_img,face_feature);       //计算出人脸的特征，并存放到face_feature.

			chensiyu_feature[face_cnt] = face_feature.clone();

			name = p.c_str()+strlen(faces_dir);
			for(i=0;*(name+i) != '.';i++)
				names[face_cnt][i]=*(name + i);
			names[face_cnt][i] = '\0';
			cout<< names[face_cnt] << endl;
			face_cnt++;



		}
}



//识别人脸mplayer_init()
string face_recongnize(Mat face_img,Mat face_box)
{
	Mat aligned_img,face_feature;
	int i;
	double cosine_score;

	fr ->alignCrop(face_img,face_box,aligned_img); //对人像进行对齐和裁减处理，并且将结果存放到aligned_img中
	fr ->feature(aligned_img,face_feature);       //计算出人脸的特征，并存放到face_feature中
	for(i = 0;i < face_cnt;i++)
	{
	cosine_score = fr ->match(face_feature,chensiyu_feature[i]);  //计算两个特征向量的相似度（采用离弦距离，值越大相似）
	if(cosine_score > 0.363)
	return names[i];
	}
	return "unkonw";
}





//按键处理函数
void keyuser(int fps,int cw,int ch)
{     
      
	VideoWriter v;
	int fourcc = VideoWriter::fourcc('m', 'p', '4', 'v');

	int keycode = waitKey(1000/60);	
	int photo_cnt = 0,video_cnt = 1;
	char image_photo[200];

	Size frameSize(640,480);


	if(keycode  == 'q')  destroyAllWindows();
	if(keycode == 32)
		{
			sprintf(image_photo,"/home/csy2022/day4/code1/%d.jpg",photo_cnt);
		        imwrite(image_photo,img);
		        // printf("已拍摄\n");
			photo_cnt++;
		}

        if(keycode == 'v')
	{
		if(video_cnt %2 != 0)
		{
				bool ret = v.open("/home/csy2022/day4/code1/video/abc.mp4",fourcc,fps, Size(640, 480));

				cout << ret << endl;

				cout << "test..." << endl;
		}
		else
		{
			v.release();
			cout << "yes"  << endl;

		}
			video_cnt++;

		}
                if(video_cnt %2 == 0)
                {
                        v.write(img);
                        putText(img,"Recording",Point(0,100),FONT_HERSHEY_SIMPLEX ,1.0,Scalar(255,0,0),1.4);

                }

}

//播放器初始化
FILE* mplayer_init(void)
{
	FILE* fp = popen("mplayer -slave -idle -really-quiet -nolirc","w");
	return fp;
}

//播放音乐
void mplayer_play(FILE* fp,const char* music_path)
{
	fprintf(fp,"load %s\n",music_path);
	fflush(fp);
}

//停止播放
void mplayer_pause(FILE* fp)
{
	fprintf(fp,"p\n");
	fflush(fp);
}

//继续播放
void mplayer_continue(FILE* fp)
{
	fprintf(fp,"p\n");
	fflush(fp);
}

//停止播放
void mplayer_stop(FILE* fp)
{
	fprintf(fp,"stop\n");
	fflush(fp);
}

//设置音量
void mplayer_set(FILE* fp,int volume)
{
	fprintf(fp,"volume\n",volume);
	fflush(fp);
}

//结束 mplayer
void mplayer_exit(FILE* fp)
{
	fprintf(fp,"q\n");
	fflush(fp);
	pclose(fp);
}


	//显示文字
		//putText(img,"csy",Point(50,100),FONT_HERSHEY_SIMPLEX ,1.0,Scalar(255,0,0),1.4);
	
		
	         // watermark=imread("/home/csy2022/day4/code1/watermark.png");
		  //addWeighted(watermark,0.4,img,0.6,0.3,img);




		//namedWindow("girl", WINDOW_KEEPRATIO);
		//resizeWindow("girl", 400, 300);
	
		// 绘制一条线	
		//line(img, Point(0, 0), Point(640, 480), Scalar(0, 255, 0), 5);
		//line(img, Point(320, 100), Point(160, 380), Scalar(0, 255, 255), 3);
		//line(img, Point(320, 100), Point(480, 380), Scalar(255, 255, 0), 3);
		//line(img, Point(160, 380), Point(480, 380), Scalar(255, 0, 255), 3);

		// 绘制一个矩形
		//rectangle(img, Rect(140, 60, 360, 340), Scalar(255, 0, 0), 5);
		//rectangle(img, Rect(280, 280, 80, 40), Scalar(0, 100, 0), -1);  // 填充矩形区域

		// 绘制一个圆圈
		//circle(img, Point(320,100), r, Scalar(0, 0, 255), 3);
                /*
		if(flag) r += 3;
		else r -= 3;

		if(r <= 0) flag = 1, r = 0;
		if(r >= 255) flag = 0, r = 255;
                */


