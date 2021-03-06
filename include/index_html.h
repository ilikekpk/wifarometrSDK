const char index_html[] = \
"<html>"\
      "<meta http-equiv=\"Content-Type\" content=\"text/html charset=utf-8\">"\
     "<meta name=\"viewport\" content=\"width=device-width\">"\
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no\">"\
   "<head>"\
         "<title>ESP8266 Settings</title>"\
                    "<style>"\
             "body"\
            "{"\
                 "background-color: #cccccc;"\
                   "font-family: Arial, Helvetica, Sans-Serif;"\
                 "Color: #000088;"\
            "}"\
        "</style>"\
   "</head>"\
   "<body>"\
        "<form method=\"POST\" action=\"/\">"\
                   "<h3 align=\"center\"><b>W I F A R O M E T R</b></h3>"\
          "<table align=\"center\">"\
            "<tr>"\
                 "<td colspan=\"3\" height=\"30\"><b>Отображение</b></td>"\
            "</tr>"\
            "<tr>"\
               "<td>"\
                   "<select name=\"first_msg\">"\
                     ""\
                  "</select>"\
                  ":"\
               "</td>"\
                 "<td><input name=\"first_msg_timer\" value=\"\"></td>"\
                "<td> сек.</td>"\
            "</tr>"\
            "<tr>"\
               "<td>"\
                   "<select name=\"second_msg\">"\
                     ""\
                  "</select>"\
                  ":"\
               "</td>"\
                "<td><input name=\"second_msg_timer\"></td>"\
                "<td> сек.</td>"\
            "</tr>"\
            "<tr>"\
               "<td>"\
                   "<select name=\"third_msg\">"\
                     ""\
                  "</select>"\
                  ":"\
               "</td>"\
                "<td><input name=\"third_msg_timer\"></td>"\
                "<td> сек.</td>"\
            "</tr>"\
            "<tr>"\
               "<td>"\
                   "<select name=\"fourth_msg\">"\
                     ""\
                  "</select>"\
                  ":"\
               "</td>"\
                "<td><input name=\"fourth_msg_timer\"></td>"\
                "<td> сек.</td>"\
            "</tr>"\
            "<tr>"\
                 "<td colspan=\"3\" height=\"30\"><b>Индикация</b></td>"\
            "</tr>"\
            "<tr>"\
                   "<td>Тускло (0 - ...): </td>"\
                "<td><input name=\"dark_adc\"></td>"\
               "<td>ед.</td>"\
            "</tr>"\
            "<tr>"\
                  "<td>Ярко(... - 1024): </td>"\
                "<td><input name=\"brigh_adc\"></td>"\
               "<td>ед.</td>"\
            "</tr>"\
            "<tr>"\
                "<td>Освещенность: </td>"\
                 "<td><input name=\"val_adc\" disabled=\"disabled\"></td>"\
               "<td>ед.</td>"\
            "</tr>"\
            "<tr>"\
                  "<td colspan=\"3\" height=\"30\"><b>Загрузка данных</b></td>"\
            "</tr>"\
            "<tr>"\
                "<td>Метеоданные: </td>"\
                "<td><input name=\"meteo_update_timer\"></td>"\
                "<td> мин.</td>"\
            "</tr>"\
            "<tr>"\
                "<td>Время: </td>"\
                "<td><input name=\"clock_update_timer\"></td>"\
                "<td> сек.</td>"\
            "</tr>"\
            "<tr>"\
                "<td>GMT: </td>"\
                "<td><input name=\"GMT\"></td>"\
               "<td>час.</td>"\
            "</tr>"\
            "<tr>"\
                  "<td colspan=\"3\" height=\"30\"><b>Координаты: </b></td>"\
            "</tr>"\
            "<tr>"\
                "<td>Широта: </td>"\
                "<td><input name=\"latitude\"></td>"\
                "<td> град.</td>"\
            "</tr>"\
            "<tr>"\
                "<td>Долгота: </td>"\
                "<td><input name=\"longitude\"></td>"\
                "<td> град.</td>"\
            "</tr>"\
            "<tr>"\
                 "<td><a href=\"/net_sets\">Настройки сети</a></td>"\
                   "<td align=\"right\" colspan=\"2\"><input type=SUBMIT value=\"Сохранить\"></td>"\
            "</tr>"\
         "</table>"\
      "</form>"\
   "</body>"\
"</html>"\
""\
"<script>"\
      "var html = `"\
       "<option value=\"disabled\">Отключено</option>"\
       "<option value=\"time\">Время</option>"\
       "<option value=\"temp\">Температура</option>"\
       "<option value=\"feels_like\">Ощущается</option>"\
        "<option value=\"temp_water\">Температура воды</option>"\
        "<option value=\"wind_speed\">Скорость ветра</option>"\
        "<option value=\"wind_gust\">Скорость порывов</option>"\
         "<option value=\"pressure_mm\">Давление мм рт</option>"\
        "<option value=\"pressure_pa\">Давление Па</option>"\
       "<option value=\"humidity\">Влажность</option>"\
   "`;"\
""\
         "var options_names = [\"first_msg\", \"second_msg\", \"third_msg\", \"fourth_msg\"];"\
                 "var input_names = [\"first_msg_timer\", \"second_msg_timer\", \"third_msg_timer\", \"fourth_msg_timer\", \"dark_adc\", \"brigh_adc\", \"val_adc\", \"meteo_update_timer\", \"clock_update_timer\", \"GMT\", \"latitude\", \"longitude\"];"\
         "var option_args = [\"%d\", \"%d\", \"%d\", \"%d\"];"\
                 "var input_args = [\"%d\", \"%d\", \"%d\", \"%d\", \"%d\", \"%d\", \"%d\", \"%d\", \"%d\", \"%d\", \"%s\", \"%s\"];"\
    "var element;"\
""\
          "for(var i = 0; i < 4; i++)"\
   "{"\
        "document.getElementsByName(options_names[i])[0].innerHTML = html;"\
        "document.getElementsByName(options_names[i])[0].getElementsByTagName(\"*\")[option_args[i]].selected = true;"\
   "}"\
""\
             "for(var i = 0; i < 12; i++) document.getElementsByName(input_names[i])[0].value = input_args[i];"\
""\
          "if(input_args[9] > 0) document.getElementsByName(\"GMT\")[0].value = \"+\" + input_args[9];"\
""\
"</script>"\
"";
