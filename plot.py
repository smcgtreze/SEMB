import csv
import numpy as np
import pandas as pd
from collections import Counter
from matplotlib import pyplot as plt

plt.style.use("fivethirtyeight")

with open('info.csv') as csv_file:
    csv_reader = csv.DictReader(csv_file)

    language_counter = Counter()

    for row in csv_reader:
        language_counter.update(row['LanguagesWorkedWith'].split(';'))

# data = pd.read_csv('data.csv')
# ids = data['Responder_id']
# lang_responses = data['LanguagesWorkedWith'] 

# language_counter = Counter()

# for response in lang_responses:
#     language_counter.update(response.split(';'))

languages = []
popularity = []

for item in language_counter.most_common(5):
    languages.append(item[0])
    popularity.append(item[1])

languages.reverse()
popularity.reverse()

print(languages)
print(popularity)

plt.barh(languages, popularity)

plt.title("Most Popular Languages")
plt.ylabel("Programming Languages")
plt.xlabel("Number of Users")

plt.tight_layout()

plt.show()