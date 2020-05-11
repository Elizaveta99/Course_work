from __future__ import absolute_import, division, print_function, unicode_literals

import os
import numpy as np
import tensorflow as tf
import h5py
import pandas as pd
import seaborn as sns
from scipy import stats

from tensorflow import keras
from tensorflow.keras import layers, models
from tensorflow.keras.utils import to_categorical
import matplotlib.pyplot as plt

from tensorflow.keras.models import Sequential
from tensorflow.keras.models import load_model
from tensorflow.keras.layers import Dense, Conv1D, Flatten, Dropout, MaxPooling1D, Reshape, GlobalAveragePooling1D, GlobalMaxPooling1D
from keras.utils import to_categorical

from sklearn import metrics
from sklearn import preprocessing

import sys

def feature_normalize(dataset):

    mu = np.mean(dataset)
    sigma = np.std(dataset)
    
    return (dataset - mu) / sigma

def main(class_name):
    base_path = 'C:\\Users\\Professional\\Desktop\\1studing\\CourseWork3\\course_work_app\\Course_work\\cnn_code_4_classes'
    model_save_path = os.path.join(base_path, 'model_temp\\cnn_model.h5')
    model_weights_path = os.path.join(base_path, 'model_temp\\cnn_model_weights.hdf5')
    predict_path = os.path.join(base_path, 'data_files_predict_4')
    
    #class_name = '221'
    path_predict = os.path.join(predict_path, class_name)
    X_pred = np.fromfile(path_predict, dtype=int, count=-1, sep=' ', offset=0)

    temp_predict = np.split(X_pred, X_pred.size // 1080, 0) # отрезки по 3 сек(1080 сэмплов) - 600 отр по 3 сек (из 648000 значений)
    
    XX_pred = np.asarray(temp_predict)
    XX_pred = XX_pred.astype("float32")
    
    
    X_predict = feature_normalize(XX_pred)
    
    model_test = load_model(model_save_path)
    model_test.load_weights(model_weights_path) 
    
    Y_predict = model_test.predict_proba(X_predict) #для кажд отр-ка - массив из 4 значений(для каждого класса) - вер-сть этого класса в этом отрезке
    
    commom_len = len(Y_predict) 
    
    LABELS = ["N", "RBBB", "B", "AFIB"]
    
    Y_pred_test = model_test.predict(X_predict)

    max_Y_pred_test = np.argmax(Y_predict, axis=1) # in each row - в каждой строчке выбираю макс вероятнсть(номер класса с макс вер-ю)
    
    class_mapping_predict = {0: 'N', 1: 'RBBB', 2: 'B', 3: 'AFIB'}
    class_counts_predict = {0: 0, 1: 0, 2: 0, 3: 0}
    
    for class_i in max_Y_pred_test:
        class_counts_predict[class_i] += 1
    
    #print('Probabilities')
    ans = ""
    for class_i in class_counts_predict:
         ans += class_mapping_predict[class_i] + ' ' + str(round(class_counts_predict[class_i] / commom_len * 100, 2)) + ' '

    file = open("C:\\Users\\Professional\\Desktop\\1studing\\CourseWork3\\course_work_app\\Course_work\\cnn_code_4_classes\\EcgApp\\EcgApp\\App_Data\\sectorsFile.txt", "w").close() # mode ?
    file = open("C:\\Users\\Professional\\Desktop\\1studing\\CourseWork3\\course_work_app\\Course_work\\cnn_code_4_classes\\EcgApp\\EcgApp\\App_Data\\sectorsFile.txt", "a") # mode ?

    for i in range(0, len(Y_predict)):
        mx = -1
        idx = -1
        for j in range(0, 4):
            if (Y_predict[i][j] > mx):
                mx = Y_predict[i][j]
                idx = j
        file.write(str(i * 600) + ':' + str(i * 600 + 600) + ':' + str(idx) + ';')

    file.close()
    print("Answer " + ans)

main(sys.argv[1])       