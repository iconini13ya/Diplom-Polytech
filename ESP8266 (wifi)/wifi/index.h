const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset='utf-8'>
    <title>Sustem Arduino</title>
    <style>
        .wrapper {
    display: grid;
    grid-template-columns: 1fr 3fr;
    grid-auto-rows: minmax(100px,auto);
    grid-row-gap: 1em;
}

.box2 {
    grid-column-start: 1;
    grid-column-end: 3;
    grid-row-start: 1;
    grid-row-end: 2;
    text-align: center;
    display: grid;
    grid-template-columns: 1fr;
    display: flex;
    align-items: center;
    justify-content: center;
    padding-bottom: 20px;
}
.box2 div{
    font-size:4vw;
    font-weight: bold;
}

.box3 {
    grid-column-start: 1;
    grid-column-end: 2;
    grid-row-start: 2;
    grid-row-end:   3;
    text-align: center;
    display: grid;
    grid-template-rows: 1fr 1fr; 
    font-size: 22px;
    grid-auto-rows: 200px;
}

.box4 {
    grid-column-start: 2;
    grid-column-end: 3;
    grid-row-start: 2;
    grid-row-end: 3;
    display: grid;
    font-size: 4vw;
    text-align: center;
    align-items: center;
    justify-content: center;
    font-weight: bold;
}

.box5{
    grid-column-start: 1;
    grid-column-end: 2;
    grid-row-start: 3;
    grid-row-end: 4;
}

.box6{
    grid-column-start: 2;
    grid-column-end: 3;
    grid-row-start: 3;
    grid-row-end: 4;
}

body{
    background-color: lightgray;
}
.logo{
    width: 100%;
}
.menue{
    font-weight: bold;
    display: flex;
    align-items: center;
    justify-content: center;
}

div.button7 {
    font-weight: 700;
    color: white;
    text-decoration: none;
    padding: .8em 1em calc(.8em + 3px);
    border-radius: 3px;
    background: rgb(64,199,129);
    box-shadow: 0 -3px rgb(53,167,110) inset;
    transition: 0.2s;
    height: 50%;
  } 
  div.button7:hover { background: rgb(53, 167, 110); }
  div.button7:active {
    background: rgb(33,147,90);
    box-shadow: 0 3px rgb(33,147,90) inset;
  }

  div.menue {
    display: inline-block;
    color: white;
    font-weight: 700;
    text-decoration: none;
    user-select: none;
    padding: .5em 2em;
    outline: none;
    border: 2px solid;
    border-radius: 1px;
    transition: 0.2s;
    height: 50%;
    background-color: rgba(10, 10, 10, 0.178);
  } 
  div.menue:hover { background: rgba(255,255,255,.2); }
  div.menue:active { background: white; }
  table{
      margin: auto;
  }
  td{
      /* border: 3px black solid; */
      padding: 10px;
  }
  .button {
    background-color: #f44336;
    border: none;
    color: white;
    padding: 15px 32px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 16px;
    margin: 4px 2px;
    cursor: pointer;
}
input{
    text-align: center;
    background-color: lightgray;
    border: none;
    outline:none;
}
input:hover{
    cursor: default;
}
.box5{
    grid-column-start: 1;
    grid-column-end: 2;
    grid-row-start: 3;
    grid-row-end: 4;
}

.box6{
    grid-column-start: 2;
    grid-column-end: 3;
    grid-row-start: 3;
    grid-row-end: 4;
}

    </style>
    <script>
        function gotoPhoneSettings() {
            document.location.href ="/phoneSettings";
        }
        function gotoMainPage(){
        document.location.href ="/";
        }
        function deleteSensor(element){
            if(confirm("Вы уверены, что ходите удалить датчик с id "+element.id+"?")){
            element.closest("tr").firstElementChild.firstElementChild.submit();
            }
        }
    </script>
</head>
<body>
    <div class="wrapper">
        <div class="box2">
            <div >
                Этот сайт предназначен для управления устройством на базе микроконтроллера Arduino 
            </div>      
        </div>
        <div class="box3">
            <div class="menue" onclick="gotoMainPage()">
                Главное меню
            </div>
            <div class="button7" onclick="gotoPhoneSettings()">
                Настройка телефона
            </div>
        </div>
        <div class="box4">
            <div class="sensorCaption">
                Подключенные датчики
            </div>
        </div>
        <div class="box5">
        </div>
        <div class="box6">
            <table>
                <thead>
                    <tr>
                        <th>
                            ID датчика
                        </th>
                        <th>
                            Тип датчика
                        </th>
                        <th>
                            Команда
                        </th>
                    </tr>
                </thead>
                <tbody>
                    @@Sensors@@
                </tbody>
            </table>
        </div>
     </div>
</body>
</html>
)=====";




const char PHONE_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <title>Sustem Arduino</title>
    <style>
        .wrapper {
    display: grid;
    grid-template-columns: 1fr 3fr;
    grid-auto-rows: minmax(100px,auto);
    grid-row-gap: 1em;
}

.box2 {
    grid-column-start: 1;
    grid-column-end: 3;
    grid-row-start: 1;
    grid-row-end: 2;
    text-align: center;
    display: grid;
    grid-template-columns: 1fr;
    display: flex;
    align-items: center;
    justify-content: center;
    padding-bottom: 20px;
}
.box2 div{
    font-size:4vw;
    font-weight: bold;
}

.box3 {
    grid-column-start: 1;
    grid-column-end: 2;
    grid-row-start: 2;
    grid-row-end:   3;
    text-align: center;
    display: grid;
    grid-template-rows: 1fr 1fr; 
    font-size: 22px;
    grid-auto-rows: 200px;
}

.box4 {
    grid-column-start: 2;
    grid-column-end: 3;
    grid-row-start: 2;
    grid-row-end: 3;
}

.box5{
    grid-column-start: 1;
    grid-column-end: 2;
    grid-row-start: 3;
    grid-row-end: 4;
}

.box6{
    grid-column-start: 2;
    grid-column-end: 3;
    grid-row-start: 3;
    grid-row-end: 4;
}

body{
    background-color: lightgray;
}
.logo{
    width: 100%;
}
.menue{
    font-weight: bold;
    display: flex;
    align-items: center;
    justify-content: center;
}

div.button7 {
    font-weight: 700;
    color: white;
    text-decoration: none;
    padding: .8em 1em calc(.8em + 3px);
    border-radius: 3px;
    background: rgb(64,199,129);
    box-shadow: 0 -3px rgb(53,167,110) inset;
    transition: 0.2s;
    height: 50%;
  } 
  div.button7:hover { background: rgb(53, 167, 110); }
  div.button7:active {
    background: rgb(33,147,90);
    box-shadow: 0 3px rgb(33,147,90) inset;
  }

  div.menue {
    display: inline-block;
    color: white;
    font-weight: 700;
    text-decoration: none;
    user-select: none;
    padding: .5em 2em;
    outline: none;
    border: 2px solid;
    border-radius: 1px;
    transition: 0.2s;
    height: 50%;
    background-color: rgba(10, 10, 10, 0.178);
  } 
  div.menue:hover { background: rgba(255,255,255,.2); }
  div.menue:active { background: white; }
  .box4{
    display: grid;
    font-size: 4vw;
    text-align: center;
    align-items: center;
    justify-content: center;
}
.box4 div{
    margin-top: 40px;
}
.phoneLabel{
    font-weight: bold;
}
.mainPhone{
    display: grid;
    margin-bottom: 20px;
}
.additionalPhone{
    display: grid;
    margin-top: 20px;
}
.box6{
    display: grid;
    font-size: 3vw;
    text-align: center;
    align-items: center;
    justify-content: center;
}
.box6 input{
    height: 30px;
    width: 80%;
    font-size: 20px;
    text-align: center;
}

.box7{
    display: grid;
    grid-template-columns: 1fr 1fr;
    margin-top: 20px;
}
.box7 input{
    background-color:lightgreen;
    outline: none;
    border: 1px black solid;
}
.deleteButton{
    background-color: lightcoral;
    font-size: 22px;
    border: 1px black solid;
    height: 28px;
    width: 80%;
    display:inline-block;
    margin: auto; 
}
.deleteButton:hover{
    cursor: default;
}
#obrazec{
    font-size: 25px;
}
    </style>
    <script>
        function gotoPhoneSettings() {
            document.location.href ="/phoneSettings";
        }
        function gotoMainPage(){
        document.location.href ="/";
        }
        function deleteMainPhone(){
            document.location.href ="/deleteMainPhone";  
        }
        function deleteAdditionalPhone(){
            document.location.href ="/deleteAdditionalPhone";  
        }
        function validPhone(Phonetype){
            var re = /\+7\d{10}/;
            if (Phonetype == "Main"){
                var myPhone = document.getElementById('mainPhone').value;
                var myForm = document.getElementById("mainForm");
            }else{
                var myPhone = document.getElementById('addPhone').value;
                var myForm = document.getElementById("addForm");
            }
            var valid = re.test(myPhone);
            if (valid) {
                myForm.submit();
            }
            else {
                document.getElementById("obrazec").style.color="red";
            }
            return valid;
        }
    </script>
</head>
<body>
    <div class="wrapper">
        <div class="box2">
            <div>
                Этот сайт предназначен для управления устройством на базе микроконтроллера Arduino 
            </div>      
        </div>
        <div class="box3">
            <div class="menue" onclick="gotoMainPage()">
                Главное меню
            </div>
            <div class="button7" onclick="gotoPhoneSettings()">
                Настройка телефона
            </div>
        </div>
        <div class="box4">
                <div class="phoneLabel">
                    Номера телефонов
                </div>
                <div id="obrazec">
                    Введите номер телефона по образцу (+7..........)
                </div>
        </div>
        <div class="box5">

        </div>
        <div class="box6">
            <form action="mainPhoneNumber" class="mainPhone" id="mainForm">
                <div>
                    Основной номер телефона
                </div>
                <div>
                    <input id="mainPhone" type="text" name="mainPhoneNumber" value="@@P1@@">
                </div>
                <div class="box7">
                    <input  value="Добавить" onclick="validPhone(`Main`)">  
                    <div class="deleteButton" onclick="deleteMainPhone()">
                        Удалить
                    </div>
                </div>
            </form>
            <form action="additionalPhoneNumber" class="additionalPhone" id="addForm">
                <div>
                    Дополнительный номер телефона
                </div>
                <div>
                    <input id="addPhone" type="text" name="additionalPhoneNumber" value="@@P2@@">
                </div>
                <div class="box7">
                    <input  value="Добавить" onclick="validPhone(`Add`)">  
                    <div class="deleteButton" onclick="deleteAdditionalPhone()">
                        Удалить
                    </div>
                </div>
            </form>
        </div>
     </div>
</body>
</html>
)=====";
