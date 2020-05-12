import csv
import numpy as np
import pandas as pd
from collections import Counter
from matplotlib import pyplot as plt

plt.style.use("fivethirtyeight")

NTasks = []
Sched = []
Util = []
TasksInSet = []
TasksInSet2 = []
Sched2 = []

with open('Dist_Util.csv',newline='') as csv_file:
    csv_reader = csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        NTasks.append(row[0])
        Util.append(row[1])



    #language_counter = Counter()

    # for row in csv_reader:
        #language_counter.update(row['Number of tasks per task set'].split(';'));

with open('LUB.csv',newline='') as csv_file:
    csv_reader = csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        Sched.append(row[0])  
        TasksInSet.append(row[1])     

with open('Hyperbolic.csv',newline='') as csv_file:
    csv_reader = csv.reader(csv_file,delimiter=',')
    for row in csv_reader:
        Sched2.append(row[0])  
        TasksInSet2.append(row[1])             

# data = pd.read_csv('data.csv')
# ids = data['Responder_id']
# lang_responses = data['LanguagesWorkedWith'] 

# language_counter = Counter()

# for response in lang_responses:
#     language_counter.update(response.split(';'))

# for item in language_counter.most_common(5):
#     NTasks.append(item[0])
#     Sched.append(item[1])

NTasks.reverse()
Util.reverse()
#TasksInSet.reverse()
Sched.reverse()

# print(NTasks)
# print(Util)
# print(TasksInSet)
# print(Sched)

plt.figure(1)
plt.barh(Util, NTasks)

plt.xticks(fontsize=11)
plt.yticks(fontsize=11)

plt.title("Utilization Distribution")
plt.ylabel("Utilization")
plt.xlabel("Number of Tasks")

plt.figure(2)
plt.plot(TasksInSet,Sched)
plt.title("Schedulability according to LUB")
plt.ylabel("Acceptance Ratio")
plt.xlabel("Number of Tasks per Set")

plt.figure(3)
plt.plot(TasksInSet2,Sched2)
plt.title("Schedulability according to HC")
plt.ylabel("Acceptance Ratio")
plt.xlabel("Number of Tasks per Set")


plt.tight_layout()
plt.show()
