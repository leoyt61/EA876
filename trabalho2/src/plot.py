import numpy as np
from basic_units import secs
import matplotlib.pyplot as plt
import math


N = int(input())

linear = open("doc/linear.txt", "r")
thread = open("doc/thread.txt", "r")
process = open("doc/process.txt", "r")

linearL = linear.readlines()
threadL = thread.readlines()
processL = process.readlines()
types = []
images = []
means = []
sds = []
for line in linearL:
    data = line.split("|")
    types.append(data[0])
    images.append(data[1])
    times = data[2].split(" ")
    times.pop()
    soma = 0
    for t1 in times:
        soma+=float(t1)
    mean = soma/len(times)
    soma2 = 0
    for t2 in times:
        soma2+= (float(t2)*float(t2) - mean*mean)
    variancia = soma2/len(times)
    sd = math.sqrt(variancia)
    means.append(mean)
    sds.append(sd)


linearMeans = tuple(means)
linearStd = tuple(sds)

fig, ax = plt.subplots()

ind = np.arange(N)    # the x locations for the groups
width = 0.25         # the width of the bars
p1 = ax.bar(ind, linearMeans, width, bottom=0.001, yerr=linearStd)


types = []
images = []
means = []
sds = []
for thread in threadL:
    data = thread.split("|")
    types.append(data[0])
    images.append(data[1])
    times = data[2].split(" ")
    times.pop()
    soma = 0
    for t1 in times:
        soma+=float(t1)
    mean = soma/len(times)
    soma2 = 0
    for t2 in times:
        soma2+= (float(t2)*float(t2) - mean*mean)
    variancia = soma2/len(times)
    sd = math.sqrt(variancia)
    means.append(mean)
    sds.append(sd)


threadMeans = tuple(means)
threadStd = tuple(sds)


threadMeans = tuple(means)
threadStd = tuple(sds)
p2 = ax.bar(ind + width, threadMeans, width, bottom=0.001, yerr=threadStd)


types = []
images = []
means = []
sds = []
for process in processL:
    data = process.split("|")
    types.append(data[0])
    images.append(data[1])
    times = data[2].split(" ")
    times.pop()
    soma = 0
    for t1 in times:
        soma+=float(t1)
    mean = soma/len(times)
    soma2 = 0
    for t2 in times:
        soma2+= (float(t2)*float(t2) - mean*mean)
    variancia = soma2/len(times)
    sd = math.sqrt(variancia)
    means.append(mean)
    sds.append(sd)


processMeans = tuple(means)
processStd = tuple(sds)
p3 = ax.bar(ind + width + width, processMeans, width, bottom=0.001, yerr=processStd)

ax.set_title('Tempo de execução')
ax.set_xticks(ind + width)



ax.set_xticklabels(tuple(images))

ax.set_yscale('log')
ax.set_ylabel('Segundos')

ax.legend((p1[0], p2[0], p3[0]), ('Linear', 'Thread', 'Process'))
ax.autoscale_view()


def autolabel(rects):
    """Attach a text label above each bar in *rects*, displaying its height."""
    for rect in rects:
        height = float(format(rect.get_height(),".4f"))
        ax.annotate('{}'.format(height),
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(0, 3),  # 3 points vertical offset
                    textcoords="offset points",
                    ha='center', va='bottom')


autolabel(p1)
autolabel(p2)
autolabel(p3)

plt.savefig('doc/media.pdf')