import os 
import cv2

"""视频分帧代码（成功）"""

videos_src_path = 'video'
videos_save_path = 'image'
 
videos = os.listdir(videos_src_path)
#videos.sort(key=lambda x:int(x[5:-4]))
 
i = 1
 
for each_video in videos:
	if not os.path.exists(videos_save_path + '/' + str(i)):
		os.mkdir(videos_save_path + '/' + str(i))
	each_video_save_full_path = os.path.join(videos_save_path,str(i))+'/'
	each_video_full_path = os.path.join(videos_src_path,each_video)
	cap = cv2.VideoCapture(each_video_full_path)
	frame_count = 1
	success = True
	
	while(success):
		success,frame = cap.read()
		if success==True:
			cv2.imwrite(each_video_save_full_path + "frame%d.jpg" % frame_count,frame)
		frame_count = frame_count + 1
	i = i + 1
	
	cap.release()