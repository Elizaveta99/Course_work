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
#from sklearn.metrics import classification_report
from sklearn import preprocessing

import sys

def feature_normalize(dataset):

    mu = np.mean(dataset)
    sigma = np.std(dataset)
    
    return (dataset - mu) / sigma

def main(class_name):
    base_path = 'C:\\Users\\Professional\\Desktop\\1studing\\CourseWork3\\course_work_app\\cnn_code_4_classes'
    model_save_path = os.path.join(base_path, 'model\\cnn_model.h5')
    model_weights_path = os.path.join(base_path, 'model\\cnn_model_weights.hdf5')
    predict_path = os.path.join(base_path, 'data_files_predict_4')
    
    #class_name = '221'
    path_predict = os.path.join(predict_path, class_name)
    X_pred = np.fromfile(path_predict, dtype=int, count=-1, sep=' ', offset=0)
    
    temp_predict = np.split(X_pred, X_pred.size // 1080, 0) 
    
    XX_pred = np.asarray(temp_predict)
    XX_pred = XX_pred.astype("float32")
    
    
    X_predict = feature_normalize(XX_pred)
    
    #print(X_predict)
    #print('--------------------------------------')
    
    model_test = load_model(model_save_path)
    model_test.load_weights(model_weights_path) 
    
    Y_predict = model_test.predict_proba(X_predict)
    
    commom_len = len(Y_predict) 
    #print(len(Y_predict))
    
    #print(Y_predict)
    
    
    #print("\n--- Confusion matrix for test data ---\n")
    
    LABELS = ["N", "RBBB", "B", "AFIB"]
    
    Y_pred_test = model_test.predict(X_predict)
    
    max_Y_pred_test = np.argmax(Y_pred_test, axis=1)
    
    #print('max')
    #print(max_Y_pred_test)
    
    class_mapping_predict = {'N': 0, 'RBBB': 1, 'B': 2, 'AFIB': 3}
    class_counts_predict = {'N': 0, 'RBBB': 0, 'B': 0, 'AFIB': 0}
    class_mapping_predict = {'N': 0, 'RBBB': 0, 'B': 0, 'AFIB': 0}
    
    class_mapping_predict = {0: 'N', 1: 'RBBB', 2: 'B', 3: 'AFIB'}
    class_counts_predict = {0: 0, 1: 0, 2: 0, 3: 0}
    
    for class_i in max_Y_pred_test:
        class_counts_predict[class_i] += 1
    
    #print('Probabilities')
    ans = "";
    for class_i in class_counts_predict:
        #print(class_mapping_predict[class_i] + ' ' + str(class_counts_predict[class_i]) + ' ' + str(class_counts_predict[class_i] / commom_len * 100))
         #print(class_mapping_predict[class_i] + ' ' + str(class_counts_predict[class_i] / commom_len * 100))
         ans += class_mapping_predict[class_i] + ' ' + str(class_counts_predict[class_i] / commom_len * 100) + ' ';
    print("Answer " + ans);

main(sys.argv[1])       