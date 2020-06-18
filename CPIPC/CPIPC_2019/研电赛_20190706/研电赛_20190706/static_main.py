# 研电赛 2019-6-20
from multiprocessing import Process, Queue, Pool, Manager, Lock, Pipe
import numpy as np
import os, time, random, cv2
import camera_configs
import pygame
import threading
import serial
import matplotlib.pyplot as plt
import datetime

# 串口读取 @WZ
class ComThread:
    def __init__(self, Port='COM3'):
        self.l_serial = None
        self.alive = False
        self.waitEnd = None
        self.port = Port
        self.direction = 0

    def waiting(self):
        if not self.waitEnd is None:
            self.waitEnd.wait()

    def start(self):
        self.l_serial = serial.Serial()
        self.l_serial.port = self.port
        self.l_serial.baudrate = 9600
        self.l_serial.open()
        if self.l_serial.isOpen():
            self.waitEnd = threading.Event()
            self.alive = True
            self.thread_read = None
            self.thread_read = threading.Thread(target=self.FirstReader)
            self.thread_read.setDaemon(1)
            self.thread_read.start()
            return True
        else:
            return False

    def FirstReader(self):
        while self.alive:
            data = ''
            data = data.encode('utf-8')

            n = self.l_serial.inWaiting()
            if n:
                 data = data + self.l_serial.read(n)

            n = self.l_serial.inWaiting()
            if len(data)>0 and n==0:
                try:
                    temp = data.decode('gb18030')
                    arr1 = temp.split('：')
                    arr2 = arr1[1].split('\r')[:1]
                    self.direction = int(arr2[0])
                    
                except:
                    print("读取错误，请重试！\n")

            flag = False
            if flag == True:
                break

            if self.direction > 0:
                flag = True

# 方向盘数据读取 @WZ
def steeringWheelRead(bg_send):
    rt = ComThread()
    rt.start()
    print(rt.l_serial.name)
    while True:
        a = int((rt.direction + 4) / 10) * 10
        bg_routine = repr(a) + '.bmp'
        time.sleep(0.1)
        bg_send.put(bg_routine)
    rt.waiting()

class itemImg:
    def __init__(self):
        self.Left = np.empty(shape=[0, 2])
        self.Right = np.empty(shape=[0, 2])
        self.imgL = np.empty(shape=[0, 2])
        self.imgR = np.empty(shape=[0, 2])
        self.DeepData = np.empty(shape=[0, 2])

# CPU压力测试
class CP:
    pass

# 距离语音播报 @WZ
def broadcast(bc_recv, people_recv):
    a = -1
    people_flag = False

    while True:
        for i in range(4):
            people_flag = False
            start = time.time()
            try:
                a = bc_recv.get()
            except:
                pass

            try:
                people_flag = people_recv.get()
            except:
                pass

            if a > 10:		a = -1
            elif a >= 10:	a = 10
            elif a >= 9:	a = 9
            elif a >= 8:	a = 8
            elif a >= 7:	a = 7
            elif a >= 6:	a = 6
            elif a >= 5:	a = 5
            elif a >= 4.5:	a = 4.5
            elif a >= 4:	a = 4
            elif a >= 3.8:	a = 3.8
            elif a >= 3.6:	a = 3.6
            elif a >= 3.4:	a = 3.4
            elif a >= 3.2:	a = 3.2
            elif a >= 3:	a = 3
            elif a >= 2.8:	a = 2.8
            elif a >= 2.6:	a = 2.6
            elif a >= 2.4:	a = 2.4
            elif a >= 2.2:	a = 2.2
            elif a >= 2:	a = 2
            elif a >= 1.9:	a = 1.9
            elif a >= 1.8:	a = 1.8
            elif a >= 1.7:	a = 1.7
            elif a >= 1.6:	a = 1.6
            elif a >= 1.5:	a = 1.5
            elif a >= 1.4:	a = 1.4
            elif a >= 1.3:	a = 1.3
            elif a >= 1.2:	a = 1.2
            elif a >= 1.1:	a = 1.1
            elif a >= 1:	a = 1
            elif a >= 0.9:	a = 0.9
            elif a >= 0.8:	a = 0.8
            elif a >= 0.7:	a = 0.7
            elif a >= 0.6:	a = 0.6
            elif a >= 0.5:	a = 0.5
            elif a >= 0.4:	a = 0.4
            elif a >= 0.3:	a = 0.3
            elif a >= 0.2:	a = 0.2
            elif a >= 0.1:	a = 0.1
            elif a >= 0:	a = 0
            else:           a = -1

            if a <= 0:
                if i == 2 and people_flag == True:
                    pass
                else:
                    continue

            if i == 2 and people_flag:
                mp3 = './' + 'people' + '.mp3'
            else:
                mp3 = './' + repr(a) + 'm.mp3'

            pygame.mixer.init()
            track = pygame.mixer.music.load(mp3)
            pygame.mixer.music.play()
            time.sleep(1)
            pygame.mixer.music.stop()
            end = time.time()

def CPUtest():
    i = 0
    while True:
        i = i + 1

# adding code-行人检测-判断目标及加矩形窗 @NXT
# 如果矩形被完全包含在另外一个矩形中，可确定该矩形应该被丢弃
def is_inside(o, i):
    ox, oy, ow, oh = o
    ix, iy, iw, ih = i
    return ox > ix and oy > iy and ox + ow < ix + iw and oy + oh < iy + ih

def draw_person(image, person):
    x, y, w, h = person
    # cv2.rectangle(img, (x, y), (x+w, y+h), (0,255,255), 2)
    cv2.rectangle(image, (x, y), (x + w, y + h), (255, 0, 255), 2)

# 摄像头读取
def read_camera(queue_camera_bm, queue_camera_full, queue_camera_show):
    # 开启摄像头
    Left = cv2.imread("../img_up/left_0.bmp")
    Right = cv2.imread("../img_up/right_0.bmp")

    pickOneOfTwo = 0

    while True:
        pickOneOfTwo = pickOneOfTwo + 1
        if pickOneOfTwo  >= 2:
            pickOneOfTwo = 0

        if pickOneOfTwo == 0:    #差值取图像
            pass
        else:
            time.sleep(0.03)
            continue
        if ret1 and ret2:
            CutImg1 = Left[60:180, 50:590]  # y x
            CutImg2 = Right[60:180, 50:590]  # y x

            # 转为灰度图
            CutImg1 = cv2.cvtColor(CutImg1, cv2.COLOR_BGR2GRAY)
            CutImg2 = cv2.cvtColor(CutImg2, cv2.COLOR_BGR2GRAY)

            # 双目畸变校正
            imgL = cv2.remap(CutImg1, camera_configs.left_map1, camera_configs.left_map2, cv2.INTER_LINEAR)
            imgR = cv2.remap(CutImg2, camera_configs.right_map1, camera_configs.right_map2, cv2.INTER_LINEAR)
        else:
            time.sleep(0.01)
            continue

        cp = CP()    #to BM
        cp.imgL = imgL
        cp.imgR = imgR

        try:    #to BM
            clear = queue_camera_bm.get_nowait()
        except:
            pass
        queue_camera_bm.put(cp)

        frame3 = cv2.imread("../img_up/up_0.bmp")

        # 单目畸变校正
        Up = cv2.remap(
                frame3, camera_configs.up_map1, camera_configs.up_map2, interpolation = cv2.INTER_LINEAR,
                borderMode = cv2.BORDER_CONSTANT
        )  

        try:    #to ObjectDetection
            clear = queue_camera_full.get_nowait()
        except:
            pass
        queue_camera_full.put(Up)

        try:    #to showimg
            clear = queue_camera_show.get_nowait()
        except:
            pass
        queue_camera_show.put(Up)

        cv2.waitKey(5)

# 显示 原始图像 距离信息 人员信息
def showImg(queue_camera_show, bm_data, Object_data, bc_send, bg_recv, people_detection):
    cv2.namedWindow("SHOW",cv2.WINDOW_AUTOSIZE)
    cv2.resizeWindow("SHOW", 800, 600);
    cv2.moveWindow("SHOW", 620, 20);
    #Depth data processing parameters 深度数据处理参数
    DDPP = [
        [0,10,0,7,0],[3,17,4,11,7],[10,24,4,11,14],
        [17,31,4,11,21],[24,39,4,11,28],[31,42,4,11,35]
    ]    

    up_zeroimg = np.zeros((600, 800 ,3), dtype=np.uint8)
    left_right_zeroimg = np.zeros((360, 640 ,3), dtype=np.uint8)

    DeepData  = np.zeros((42), dtype=np.uint16)
    show_img = up_zeroimg.copy()
    Deepimg = left_right_zeroimg.copy()
    personImg = up_zeroimg.copy()
    original_img = up_zeroimg.copy()
    objectImg = up_zeroimg.copy()

    # 20帧距离信息
    distance = []

    # 单帧距离信息
    single_distance = []

    # 帧计数
    cnt = 0

    # 统计20帧运算时间
    #time_cnt = 0
    people_cnt = 0

    # 方向盘默认处于正中间
    temp = '50.bmp'

    while True:         
        #if time_cnt == 0:
        #    start = time.time()

        #time_cnt = time_cnt + 1

        # 读取背景图片
        try:
            temp = bg_recv.get_nowait()
        except:
            pass

        BG = cv2.imread(temp)
        original_sige = 0    #get person data
        try:
            original_img = queue_camera_show.get(1)
            original_sige = 1
        except:
            pass

        Deep_sige = 0    #对应距离 = DeepData[int((x-110)/10)]
        try:
            DeepData = bm_data.get_nowait()
            Deep_sige = 1
        except:
            pass

        person_sige = 0    #get person data
        try:
            found_filtered = Object_data.get_nowait()
            person_sige = 1
        except:
            pass
       
        if person_sige:    #行人识别标记
            # 接收到的found_filtered不为空，说明检测到了行人
            # 计数+1
            if len(found_filtered) > 0:
                people_cnt = people_cnt + 1

            personImg = up_zeroimg.copy()
            for person in found_filtered:
                x1, y1, w1, h1 = person
                draw_person(personImg,(int(800 - x1 - w1), int(y1), 
                                       int(w1), int(h1)))#镜像旋转
                #x1, y1, w1, h1 = person  
                # python 中数组怎么表示。输出的是检测小的区域的矩形顶点坐标，（没有输出检测到人的坐标）
         
        if Deep_sige:
            Deepimg = left_right_zeroimg.copy()    #清空图像
            for i in range(6):
                Parameter = DDPP[i]
                cutData = DeepData[Parameter[0]:Parameter[1]]
                deepmin = cutData.min()
                if deepmin < 1400:
                    cutData = cutData[Parameter[2]:Parameter[3]]
                    tryResult = False
                    try:
                        aims = cutData.tolist().index(deepmin)
                        tryResult = True
                    except:
                        pass
                    if tryResult:
                        aims = aims+Parameter[4]
                        x_num = int(485 - aims*10)#镜像坐标 640 - 原坐标
                        showmin = deepmin/100.
                        if showmin < 3:    #3米内红色
                            colorTest = (0, 0, 255)
                        elif showmin < 5:    #5米内绿色
                            colorTest = (0, 255, 0)
                        else:    #5米外亮蓝色
                            colorTest = (255, 255, 0)
                        cv2.putText(Deepimg, "%.1fM" % (showmin),(x_num, 350), 
                                    cv2.FONT_HERSHEY_SIMPLEX, 1.0, colorTest, 3)

                        if showmin > 0:
                            single_distance.append(showmin)

        if len(single_distance):
            # 将单帧图像距离的最小值放入distance列表
            distance.append(min(single_distance))

        single_distance.clear()
        cnt = cnt + 1
        if cnt > 20:
            # 对20帧图像距离的最小值进行排序
            distance.sort()
            try:
                # 将20帧图像距离最小值的中值发送给语音播报模块
                bc_send.put(distance[int(len(distance) / 2)])
            except:
                bc_send.put(0)
                pass

            distance.clear()
            cnt = 0

            # 20帧图象中超过4帧出现行人框，代表有行人
            if people_cnt > 4:
                people_detection.put(True)
            else:
                people_detection.put(False)
            people_cnt = 0

        if person_sige or Deep_sige:
            objectImg = cv2.resize(Deepimg, (800, 600))
            objectImg = cv2.addWeighted(objectImg, 1, personImg, 1, 0)
            objectImg = cv2.addWeighted(objectImg, 1, BG, 1, 0)

        original_img = cv2.flip(src=original_img, flipCode = 1)#镜像旋转
        show_img = cv2.addWeighted(original_img, 1, objectImg, 1, 0) 
        # 下面两行代码出问题范师兄背锅
        show_img1 = cv2.resize(show_img, (1440, 1080))
        cv2.imshow("SHOW", show_img1)
        cv2.waitKey(10)
        # 统计20帧运算时间
        #if time_cnt == 20:
        #    end = time.time()
        #    print("show: ", end - start)
        #    time_cnt = 0

# 距离信息计算
def BM(queue_camera_bm, bm_data):
    num = 4
    blockSize = 10
    speckleWindowSize = 100
    # 顺序矩阵 新加入的 存0号位置
    cut_data_array = np.zeros((924, 3), dtype=np.int16)
    cv2.namedWindow("cut img", 0);
    cv2.resizeWindow("cut img", 420, 110);
    cv2.moveWindow("cut img", 55, 20);

    cv2.namedWindow("BM",0);
    cv2.resizeWindow("BM", 420, 110)
    cv2.moveWindow("BM", 55, 25+150);

    cv2.namedWindow("Sampled at intervals", 0);    #间隔采样
    cv2.resizeWindow("Sampled at intervals", 420, 110);
    cv2.moveWindow("Sampled at intervals",55,25+150*2);

    cv2.namedWindow("Noise reduction", 0);    #降噪
    cv2.resizeWindow("Noise reduction", 420, 110);
    cv2.moveWindow("Noise reduction",55,25+150*3);

    cv2.namedWindow("Sort by distance", 0);    #距离排序
    cv2.resizeWindow("Sort by distance", 420, 110);
    cv2.moveWindow("Sort by distance",55,25+150*4);
    while True:
        try:
            cp = queue_camera_bm.get(1)
        except:
            continue
        imgL = cp.imgL
        imgR = cp.imgR
        stereo = cv2.StereoBM_create(64, 11)
        #stereo = cv2.StereoSGBM_create(0, 16 * num, blockSize, 0, 0, -10, 31, 20, speckleWindowSize, 32, 0)
        disparity = stereo.compute(imgL, imgR)
        disp = cv2.normalize(disparity, disparity, alpha=0, beta=255, norm_type=cv2.NORM_MINMAX, dtype=cv2.CV_8U)
        cv2.imshow("cut img",imgL)
        cv2.imshow("BM",disp)
        # 将图片扩展至3d空间中，其z方向的值则为当前的距离
        threeD = cv2.reprojectImageTo3D(disparity.astype(np.float32) / 16., camera_configs.Q)

        # 距离数据切片  6/10  shape 22/84 float32
        cut_threeD = threeD[0:110:5, 60:480:10, 2]
        #print(cut_threeD[1])
        cut_threeD = cut_threeD / 10  # 精度为厘米
        cut_threeD[cut_threeD > 1400] = 1400  # 最大匹配距离
        cut_threeD[cut_threeD < 0] = 1500  # 无效值
        cut_threeD = cut_threeD.astype(np.uint16)  # 取整，改变数据类型
        cut_data = cut_threeD.reshape(924, 1)  # (462,1)  实时图像数据

        cut_data_show = cut_threeD.copy()  # 结果显示
        cut_data_show = cut_data_show / 5.88
        cut_data_show = cut_data_show.astype(np.uint8)
        cv2.imshow("Sampled at intervals", cut_data_show)

        cut_data_array = cut_data_array[:, 0:2]  # 剔除过期数据
        cut_data_array = np.concatenate((cut_data, cut_data_array), axis=1)  # 引入最新图片
        cut_data_sequence = cut_data_array.copy()  # 数据深拷贝
        cut_data_sequence.sort(axis=1)  # 数据排序

        cut_data_mask = cut_data_sequence[:, 2]  # 降噪过滤Mask
        cut_data_out = cut_data_sequence[:, 1]  # 中值数据
        cut_data_out[cut_data_mask > 1400] = 1500  # 过滤
        cut_data_out = cut_data_out.reshape(22, 42)  # 重新整形

        cut_data_show = cut_data_out.copy()  # 结果显示
        cut_data_show = cut_data_show / 5.88
        cut_data_show = cut_data_show.astype(np.uint8)
        cv2.imshow("Noise reduction", cut_data_show)

        cut_data_out.sort(axis=0)  # 数据排序

        # 现场演示，将两侧的数据截断
        # 只保留中间走廊信息
        #cut_data_out[:, 0 : 19] = 1500
        #cut_data_out[:, 26 : 42] = 1500

        deep_data = cut_data_out[4,:]           #最终距离数据
        deep_data = deep_data.reshape(42)

        cut_data_show = cut_data_out.copy()  # 结果显示
        cut_data_show = cut_data_show / 5.88
        cut_data_show = cut_data_show.astype(np.uint8)
        cv2.imshow("Sort by distance", cut_data_show)
        
        try:
            clear = bm_data.get_nowait()
        except:
            pass
        bm_data.put(deep_data)

        cv2.waitKey(1)

# 目标检测 @BJJ
def ObjectDetection(queue_camera_full, Object_data):
    cnt = 0

    while True:
        try:
            Left = queue_camera_full.get(1)
        except:
            continue

        img = Left
        # 检测人的默认检测器
        hog = cv2.HOGDescriptor()  
        hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())  

        # 加载图像
        found, w = hog.detectMultiScale(img)  
        found_filtered = []

        # 遍历检测结果来丢弃不含有检测目标的区域
        for ri, r in enumerate(found):
            for qi, q in enumerate(found):
                if ri != qi and is_inside(r, q):
                    break
                else:
                    found_filtered.append(r)
        
        try:
            clear = Object_data.get_nowait()
        except:
            pass
        Object_data.put(found_filtered)

if __name__ == '__main__':

    # 消息队列
    queue_camera_bm = Queue(3)  # 进程通讯
    bm_data = Queue(3)  # 进程通讯
    queue_camera_full = Queue(3)  # 进程通讯
    queue_camera_show = Queue(3)  # 进程通讯
    Object_data = Queue(3)  # 进程通讯

    # 消息队列-语音数据传送
    bc = Queue()

    # 消息队列-背景图片传送
    bg = Queue()

    # 消息队列-行人检测
    people_detection = Queue()

    #摄像头获取(sgbm专用，人员检测)
    rc = Process(target=read_camera, args=(queue_camera_bm, queue_camera_full,queue_camera_show, ))
    rc.start()

    # 景深计算
    bm = Process(target=BM, args=(queue_camera_bm, bm_data, ))
    bm.start()

    # 行人检测
    obde = Process(target=ObjectDetection, args=(queue_camera_full, Object_data, ))
    obde.start()

    # 综合显示
    show = Process(target=showImg, args=(queue_camera_show, bm_data, Object_data, bc, bg, people_detection, ))
    show.start()

    # 语音播报
    bc = Process(target=broadcast, args = (bc, people_detection, ))
    bc.start()

    # 方向盘数据读取
    bg = Process(target=steeringWheelRead, args = (bg, ))
    bg.start()

    while True:
        if cv2.waitKey(100) & 0xFF == ord('q'):
            break