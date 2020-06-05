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

with open('Dist_Util.csv',newline='') as csv_file:
    csv_reader = csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        Util.append(row[0])
        NTasks.append(row[1])

    #language_counter = Counter()

    # for row in csv_reader:
        #language_counter.update(row['Number of tasks per task set'].split(';'));

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
# data = pd.read_csv('data.csv')
# ids = data['Responder_id']
# lang_responses = data['LanguagesWorkedWith'] 

# language_counter = Counter()

# for response in lang_responses:
#     language_counter.update(response.split(';'))

# for item in language_counter.most_common(5):
#     NTasks.append(item[0])
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

# plt.figure(1)
# plt.barh(Util, NTasks)

plt.xticks(fontsize=11)
plt.yticks(fontsize=11)

# plt.title("Utilization Distribution")
# plt.ylabel("Utilization")
# plt.xlabel("Number of Tasks")

plt.figure(1)
plt.plot(TasksInSet, Sched, color='#8C2D19')
plt.title("Schedulability according to LUB")
plt.ylabel("Acceptance Ratio")
plt.xlabel("Number of Tasks per Set")
plt.gcf().autofmt_xdate()

plt.figure(2)
plt.plot(TasksInSet2,Sched2)
plt.title("Schedulability in HC")
plt.ylabel("Acceptance Ratio")
plt.xlabel("Number of Tasks per Set")
plt.gcf().autofmt_xdate()

plt.figure(3)
plt.plot(TasksInSet3,Sched3, color='#444444')
plt.title("Schedulability according to RTA")
plt.ylabel("Acceptance Ratio")
plt.xlabel("Number of Tasks per Set")
plt.gcf().autofmt_xdate()


plt.tight_layout()
plt.show()