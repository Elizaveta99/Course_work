# ECG analysis using an experimental classifier of pathologies based on the convolutional neural network<br>
## Part 2
 - [Код обучения сети](https://github.com/Elizaveta99/Course_work/blob/master/cnn_code_4_classes/training_4.ipynb)
 - [Код тестирования сети](https://github.com/Elizaveta99/Course_work/blob/master/cnn_code_4_classes/testing_4.ipynb)
 - [Код классификации](https://github.com/Elizaveta99/Course_work/blob/master/cnn_code_4_classes/predicting_4.py) - запускается из приложения. В качестве результата выводит строку с распределением вероятностей по классам патологий, а также записывает в файл координаты этих патологий на графике.
### Описание приложения
<em>Использовалась платформа ASP.NET Core</em><br />
 - Главная страница с информацией о системе - ![main](https://github.com/Elizaveta99/Course_work/blob/master/images_report/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%20(145).png)
 - Страница выбора ЭКГ-записи с компьютера, [код view - ChooseEcgRecord.cshtml](https://github.com/Elizaveta99/Course_work/blob/master/cnn_code_4_classes/EcgApp/EcgApp/Views/ChooseEcgRecord/ChooseEcgRecord.cshtml) ![choose1](https://github.com/Elizaveta99/Course_work/blob/master/images_report/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%20(146).png) <br />
![choose2](https://github.com/Elizaveta99/Course_work/blob/master/images_report/%D0%A1%D0%BD%D0%B8%D0%BC%D0%BE%D0%BA%20%D1%8D%D0%BA%D1%80%D0%B0%D0%BD%D0%B0%20(147).png)
 - 


## Part 1 (for course project, winter)
### Building<br>
<ol>
<li>Install and run jupyter notebook
  <ul>
    <li>pip3 install --upgrade pip</li>
    <li>pip3 install jupyter</li>
    <li>jupyter notebook</li>
  </ul>
</li>
<li>Install libraries : 
  <ul>
    <li>numpy</li>
    <li>h5py</li>
    <li>seaborn</li>
    <li>scipy</li>
    <li>sklearn</li>
    <li>keras.utils</li>
  </ul>
</li>
<li>For training run <strong>training.ipynb</strong></li>
<li>For testing on pretrained weights run <strong>testing.ipynb</strong></li>
</ol>
<br>
<strong>In all files change <em>base_path</em> to the place where the project is stored</strong><br><br>

### Information<br>
<em>Data extraction(received database), analysis, building dataset - in data_analysis branch</em><br>

