import csv
from matplotlib import pyplot as plt
import itertools
import numpy as np

plt.style.use("fivethirtyeight")

NTasks = []
Sched = []
Util = []
TasksInSet = []
TasksInSet2 = []
Sched2 = []
TasksInSet3 = []
Sched3 = []
TasksInSet4 = []
Sched4 = []

# with open('Dist_Util.csv',newline='') as csv_file:
#     csv_reader = csv.reader(csv_file,delimiter=',')
#     for row in csv_reader:
#         Util.append(row[0])
#         NTasks.append(row[1])

    #language_counter = Counter()

    # for row in csv_reader:
        #language_counter.update(row['Number of tasks per task set'].split(';'));

with open('info.txt', 'r') as file:
    sched = file.readline()
    var = file.readline()
    par = file.readline()
    bloc = file.readline()

with open('LUB.csv',newline='') as csv_file:
    csv_reader = csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        TasksInSet.append(float(row[0]))  
        Sched.append(float(row[1]))     

with open('Hyperbolic.csv',newline='') as csv_file:
    csv_reader = csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        TasksInSet2.append(float(row[0])) 
        Sched2.append(float(row[1]))     

with open('RTA.csv',newline='') as csv_file:
    csv_reader = csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        TasksInSet3.append(float(row[0])) 
        Sched3.append(float(row[1]))

with open('CPU_Demand.csv',newline='') as csv_file:
    csv_reader = csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        TasksInSet4.append(float(row[0])) 
        Sched4.append(float(row[1]))

# data = pd.read_csv('data.csv')
# ids = data['Responder_id']
# lang_responses = data['LanguagesWorkedWith'] 

# language_counter = Counter()

# for response in lang_responses:
#     language_counter.update(response.split(';'))

# for item in language_counter.most_common(5):
#     NTasks.append(item[0])Tasks
#     Sched.append(item[1])

# NTasks.reverse()
# Util.reverse()
#TasksInSet.reverse()
# Sched.reverse()

#TasksInSet2.reverse()

#TasksInSet3.reverse()
# Sched3.reverse()

# print(NTasks)
# print(Util)
# print(TasksInSet)
# print(Sched)

plt.xticks(fontsize=11)
plt.yticks(fontsize=11)

plt.figure(1)
plt.plot(TasksInSet, Sched, color='#8C2D19')
plt.title("Schedulability according to LUB"+" "+sched)
plt.ylabel("Acceptance Ratio")
if var == "UT\n":
    plt.xlabel("Number of Tasks per Set"+" U="+par)
if var == "TSS\n":
    plt.xlabel("Average Utilization of the Set"+" TSS="+par)
plt.gcf().autofmt_xdate()

plt.figure(2)
plt.plot(TasksInSet2,Sched2)
plt.title("Schedulability in HC"+" "+sched)
plt.ylabel("Acceptance Ratio")
if var == "UT\n":
    plt.xlabel("Number of Tasks per Set"+" U="+par)
if var == "TSS\n":
    plt.xlabel("Average Utilization of the Set"+" TSS="+par)
plt.gcf().autofmt_xdate()

plt.figure(3)
plt.plot(TasksInSet3,Sched3, color='#444444')
plt.title("Schedulability according to RTA"+" "+sched)
plt.ylabel("Acceptance Ratio"+" B"+bloc)
if var == "UT\n":
    plt.xlabel("Number of Tasks per Set"+" U="+par)
if var == "TSS\n":
    plt.xlabel("Average Utilization of the Set"+" TSS="+par)
plt.gcf().autofmt_xdate()

plt.figure(4)
plt.plot(TasksInSet4,Sched4, color='#D5FF00')
plt.title("EDF Schedulability according to CPU Demand")
plt.ylabel("Acceptance Ratio")
if var == "UT\n":
    plt.xlabel("Number of Tasks per Set"+" U="+par)
if var == "TSS\n":
    plt.xlabel("Average Utilization of the Set"+" TSS="+par)
plt.gcf().autofmt_xdate()

# plt.figure(5)
# plt.barh(NTasks,Util)
# plt.title("Utilization Distribution")
# plt.ylabel("Utilization")
# plt.xlabel("Number of Tasks")

plt.tight_layout()
plt.show()