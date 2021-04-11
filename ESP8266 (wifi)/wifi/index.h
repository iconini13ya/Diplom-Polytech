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
.box1 {
    grid-column-start: 1;
    grid-column-end: 2;
    grid-row-start: 1;
    grid-row-end: 2;   
}
.box2 {
    grid-column-start: 2;
    grid-column-end: 3;
    grid-row-start: 1;
    grid-row-end: 2;
    text-align: center;
    display: grid;
    grid-template-columns: 1fr;
    display: flex;
    align-items: center;
    justify-content: center;
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
  } 
  div.button7:hover { background: rgb(53, 167, 110); }
  div.button7:active {
    background: rgb(33,147,90);
    box-shadow: 0 3px rgb(33,147,90) inset;
  }
    </style>
</head>
<body>
    <div class="wrapper">
        <div class="box1">
            <img class="logo" src="/Logo.png">
        </div>
        <div class="box2">
            <div>
                Этот сайт предназначен для управления устройством на базе микроконтроллера Arduino 
            </div>      
        </div>
        <div class="box3">
            <div class="menue">
                Главное меню
            </div>
            <div class="button7" onclick="gotoPhoneSettings()">
                Настройка телефона
            </div>
        </div>
        <div class="box4">
            4
        </div>
     </div>
</body>
</html>
)=====";