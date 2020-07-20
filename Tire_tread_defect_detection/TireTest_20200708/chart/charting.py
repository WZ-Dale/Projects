import numpy as np
import xlrd
import matplotlib.pyplot as plt

data = xlrd.open_workbook('D:/Tire_Test/chart/data.xlsx')
table = data.sheets()[0]  # 通过索引获取
nrows = table.nrows  # 行数
ncols = table.ncols  # 列数
datamatrix = np.zeros((nrows, ncols))  # 构造数据行列式（矩阵）
for x in range(nrows):  # 有850列
    rows = table.row_values(x)  # 每列的57行值
    datamatrix[x, :] = rows
print(datamatrix.shape,type(datamatrix))
# data1 = datamatrix[0,:]
# data2 = datamatrix[1,:]
data3 = datamatrix[2,:]
# data4 = datamatrix[3,:]
# data5 = datamatrix[4,:]
data6 = datamatrix[5,:]

# label = np.linspace(0,4,5)
# data1.tolist()
# print(len(data1))
label = np.linspace(0,200,20)

# font = FontProperties(fname=r"C:\Windows\Fonts\Euclid.ttf",size=10)
# font_c = FontProperties(fname=r"C:\Windows\Fonts\simsun.ttc",size=10)



# plt.figure()
# plt.subplot(2,3,1)
# plt.bar(label,data1,facecolor='gray',edgecolor='black')
# # plt.bar(label,data2,facecolor='white',edgecolor='black',alpha=0.5)

plt.plot(label,data3,color='black',linewidth=1,linestyle='-',marker='1',label='GCN(GraphSAGE)')
plt.plot(label,data6,color='gray',linewidth=1,linestyle='-',marker='2',label='GCN')
plt.axis([0,200,0,0.04])
plt.xlabel('EPOCHS',FontProperties=font)
plt.ylabel('LOSS',FontProperties=font)
plt.yticks(fontproperties = font)
plt.xticks(fontproperties = font)
plt.legend(loc='upper left',frameon=False,prop=font)
plt.grid(ls=":",) # 网格
# plt.savefig('C:/Users/Lucky/Desktop/one7.jpg',dpi=600)
plt.show()