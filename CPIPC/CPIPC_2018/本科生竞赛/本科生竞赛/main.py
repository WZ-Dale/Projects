#coding=gbk
from multiprocessing import Process, Queue, Pool, Manager, Lock
import numpy as np
import os, time, random, cv2
import camera_configs_540_120 as camera_configs


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

# 读摄像头并显示的代码:
def read_camera(queue_camera_bm, queue_camera_full,queue_camera_show):
    # 开启摄像头
    camera1 = cv2.VideoCapture(1)
    camera2 = cv2.VideoCapture(0)
    camera1.set(3, 640)  # 设置分辨率
    camera1.set(4, 360)
    camera2.set(3, 640)  # 设置分辨率
    camera2.set(4, 360)
    
    pickOneOfTwo = 0

    SwapCamera = not True    #交换摄像头
    key = cv2.waitKey(5)
    #end = time.clock()    #间隔测速
    #start = time.clock()
    while True:
        pickOneOfTwo = pickOneOfTwo + 1
        if pickOneOfTwo  >= 2:
            pickOneOfTwo = 0
        #if not pickOneOfTwo:    #间隔测速
        #    end = time.clock()
        #    delay_time =int((end-start) * 1000)#定速延时每秒10帧刷新
        #    start = time.clock()
        #    print(delay_time)
        ret1 = 1
        ret2 = 1
        if SwapCamera:
            ret1, Left = camera1.read()
            ret2, Right = camera2.read()
        else:
            ret1, Left = camera2.read()
            ret2, Right = camera1.read()
        ##Left = cv2.imread('D:/Left_15.jpg')
        ##Right = cv2.imread('D:/Right_15.jpg')
        if pickOneOfTwo == 0:    #差值取图像
            pass
        else:
            time.sleep(0.03)
            continue
        if ret1 and ret2:
            CutImg1 = Left[60:180, 50:590]  # y x
            CutImg2 = Right[60:180, 50:590]  # y x
            # cv2.imshow("left", CutImg1)
            # cv2.imshow("right", frame2)
            CutImg1 = cv2.cvtColor(CutImg1, cv2.COLOR_BGR2GRAY)
            CutImg2 = cv2.cvtColor(CutImg2, cv2.COLOR_BGR2GRAY)
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

        try:    #to ObjectDetection
            clear = queue_camera_full.get_nowait()
        except:
            pass
        queue_camera_full.put(Left)

        try:    #to showimg
            clear = queue_camera_show.get_nowait()
        except:
            pass
        queue_camera_show.put(Left)

        #cv2.imshow("CutImg1", CutImg1)

        cv2.waitKey(5)
        #print(key)
        #if cv2.waitKey(3) & 0xFF == ord('q'):
        #    break
        #elif cv2.waitKey(2) & 0xFF == ord('s'):    #交换摄像头
        #    SwapCamera = not SwapCamera
        #    print(SwapCamera)
    

# 显示 原始图像 距离信息 人员信息
def showImg(queue_camera_show, bm_data, Object_data):
    cv2.namedWindow("SHOW",cv2.WINDOW_NORMAL)
    #cv2.namedWindow("SHOW", 0);
    cv2.resizeWindow("SHOW", 1280, 720);
    cv2.moveWindow("SHOW",620,20);
    #Depth data processing parameters 深度数据处理参数
    DDPP = [
        [0,10,0,7,0],[3,17,4,11,7],[10,24,4,11,14],
        [17,31,4,11,21],[24,39,4,11,28],[31,42,4,11,35]
    ]    #Depth data processing array

    #BG = cv2.imread("/home/pi/Desktop/BirdVision/BG.bmp")#树莓派目录
    BG = cv2.imread('./BG.jpg')#PC目录
    zeroimg = np.zeros((360,640,3), dtype=np.uint8)
    DeepData  = np.zeros((42), dtype=np.uint16)
    show_img = zeroimg.copy()
    Deepimg = zeroimg.copy()
    personImg = zeroimg.copy()
    original_img = zeroimg.copy()
    objectImg = zeroimg.copy()

    while True:         
        original_sige = 0    #get person data
        try:
            original_img = queue_camera_show.get(1)
            original_sige = 1
        except:
            pass

        original_img = cv2.flip(src=original_img,flipCode=1)#镜像旋转
        show_img = cv2.addWeighted(original_img, 1,objectImg, 1, 0) 
        cv2.imshow("SHOW", show_img)
        cv2.waitKey(10)


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
            personImg = BG.copy()    #清空图像
            #draw_person(personImg, (110,70,420,110))    #显示测距区域
            for person in found_filtered:
                x1, y1, w1, h1 = person
                #draw_person(personImg,(int(x1+w1/4), int(y1+h1*2/15), int(w1/2), int(h1*11/15)))
                draw_person(personImg,(int(640-x1-w1*3/4), int(y1+h1*2/15), int(w1/2), int(h1*11/15)))#镜像旋转
                #draw_person(personImg, person)
                #x1, y1, w1, h1 = person  # python 中数组怎么表示。输出的是检测小的区域的矩形顶点坐标，（没有输出检测到人的坐标）
         
        if Deep_sige:
            Deepimg = zeroimg.copy()    #清空图像
            #deepmax = Deepimg.min() + 200
            for i in range(6):
                Parameter = DDPP[i]
                #print(Parameter)
                cutData = DeepData[Parameter[0]:Parameter[1]]
                deepmin = cutData.min()
                if deepmin < 1400:
                    #print(deepmin)
                    cutData = cutData[Parameter[2]:Parameter[3]]
                    #print(cutData)
                    #print(cutData.tolist().index(deepmin))
                    tryResult = False
                    try:
                        aims = cutData.tolist().index(deepmin)
                        tryResult = True
                    except:
                        pass
                    if tryResult:
                        aims = aims+Parameter[4]
                        #print(aims)
                        #x_num = int(aims*10+80)
                        x_num = int(485 - aims*10)#镜像坐标 640 - 原坐标
                        showmin = deepmin/100.
                        #print(showmin)
                        if showmin < 3:    #3米内红色
                            colorTest = (0, 0, 255)
                        elif showmin < 5:    #5米内绿色
                            colorTest = (0, 255, 0)
                        else:    #5米外亮蓝色
                            colorTest = (255, 255, 0)
                        cv2.putText(Deepimg, "%.1fM" % (showmin),(x_num, 350), cv2.FONT_HERSHEY_SIMPLEX, 1.0, colorTest, 3)
                    #cv2.waitKey(0)
                #print(cutData)

        if person_sige or Deep_sige:
            objectImg = cv2.addWeighted(personImg, 1, Deepimg, 1, 0)

def BM(queue_camera_bm, bm_data):
    num = 4
    blockSize = 10
    speckleWindowSize = 100
    # 顺序矩阵 新加入的 存0号位置
    # cut_data_array = np.zeros((462*4,3), dtype=np.int32)
    cut_data_array = np.zeros((924, 3), dtype=np.int16)
    ##cv2.namedWindow("cut img", 0);
    #cv2.resizeWindow("cut img", 420, 110);
    ##cv2.moveWindow("cut img",10,20);

    ##cv2.namedWindow("BM",0);
    ##cv2.moveWindow("BM",10,25+150);

    ##cv2.namedWindow("Sampled at intervals", 0);    #间隔采样
    ##cv2.resizeWindow("Sampled at intervals", 420, 110);
    ##cv2.moveWindow("Sampled at intervals",55,25+150*2);

    ##cv2.namedWindow("Noise reduction", 0);    #降噪
    ##cv2.resizeWindow("Noise reduction", 420, 110);
    ##cv2.moveWindow("Noise reduction",55,25+150*3);

    ##cv2.namedWindow("Sort by distance", 0);    #距离排序
    ##cv2.resizeWindow("Sort by distance", 420, 110);
    ##cv2.moveWindow("Sort by distance",55,25+150*4);
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
        ##cv2.imshow("cut img",imgL)
        ##cv2.imshow("BM",disp)
        # 将图片扩展至3d空间中，其z方向的值则为当前的距离
        threeD = cv2.reprojectImageTo3D(disparity.astype(np.float32) / 16., camera_configs.Q)

        # 距离数据切片  6/10  shape 22/84 float32
        cut_threeD = threeD[0:110:5, 60:480:10, 2]
        cut_threeD = cut_threeD / 10  # 精度为厘米
        cut_threeD[cut_threeD > 1400] = 1400  # 最大匹配距离
        cut_threeD[cut_threeD < 0] = 1500  # 无效值
        cut_threeD = cut_threeD.astype(np.uint16)  # 取整，改变数据类型
        cut_data = cut_threeD.reshape(924, 1)  # (462,1)  实时图像数据

        cut_data_show = cut_threeD.copy()  # 结果显示
        cut_data_show = cut_data_show / 5.88
        cut_data_show = cut_data_show.astype(np.uint8)
        ##cv2.imshow("Sampled at intervals", cut_data_show)

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
        ##cv2.imshow("Noise reduction", cut_data_show)

        cut_data_out.sort(axis=0)  # 数据排序

        deep_data = cut_data_out[4,:]           #最终距离数据
        deep_data = deep_data.reshape(42)

        cut_data_show = cut_data_out.copy()  # 结果显示
        cut_data_show = cut_data_show / 5.88
        cut_data_show = cut_data_show.astype(np.uint8)
        ##cv2.imshow("Sort by distance", cut_data_show)
        
        try:
            clear = bm_data.get_nowait()
        except:
            pass
        bm_data.put(deep_data)

        
        #cv2.imshow("disp", disp)
        # cv2.imshow("show right", Right)
        cv2.waitKey(1)
        #time.sleep(0.05)

    


# 目标检测
def ObjectDetection(queue_camera_full, Object_data):
    while True:
        try:
            Left = queue_camera_full.get(1)
        except:
            continue
        # img=cp.Left
        img = Left
        # cv2.imshow("show Left", Left)
        # img =   #读取加载原图
        hog = cv2.HOGDescriptor()  # 检测人的默认检测器
        hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())  # SVM

        found, w = hog.detectMultiScale(img)  # 加载图像
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

        #cv2.imshow("people detection", img)
        #cv2.waitKey(1)




if __name__ == '__main__':

    # 父进程创建Queue，并传给各个子进程
    # parent_conn, child_conn = Pipe()
    queue_camera_bm = Queue(3)  # 进程通讯
    bm_data = Queue(3)  # 进程通讯
    queue_camera_full = Queue(3)  # 进程通讯
    queue_camera_show = Queue(3)  # 进程通讯
    Object_data = Queue(3)  # 进程通讯

    #摄像头获取(sgbm专用，人员检测)
    rc = Process(target=read_camera, args=(queue_camera_bm, queue_camera_full,queue_camera_show,))
    rc.start()

    bm = Process(target=BM, args=(queue_camera_bm, bm_data,))
    bm.start()

    obde = Process(target=ObjectDetection, args=(queue_camera_full, Object_data,))
    obde.start()

    show = Process(target=showImg, args=(queue_camera_show, bm_data, Object_data,))
    show.start()

    while True:
        if cv2.waitKey(100) & 0xFF == ord('q'):
            break
        #time.sleep(0.5)