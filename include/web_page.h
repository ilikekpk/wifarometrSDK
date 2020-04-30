#ifndef WEB_PAGE_H
#define WEB_PAGE_H

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
                "<td>Погода: </td>"\
                 "<td><input name=\"tmpTime\" value=\"3\"></td>"\
                "<td> сек.</td>"\
            "</tr>"\
            "<tr>"\
                "<td>Давление: </td>"\
                 "<td><input name=\"prsTime\" value=\"3\"></td>"\
                "<td> сек.</td>"\
            "</tr>"\
            "<tr>"\
                "<td>Время: </td>"\
                 "<td><input name=\"clkTime\" value=\"3\"></td>"\
                "<td> сек.</td>"\
            "</tr>"\
            "<tr>"\
                 "<td colspan=\"3\" height=\"30\"><b>Индикация</b></td>"\
            "</tr>"\
            "<tr>"\
                   "<td>Тускло (0 - ...): </td>"\
                 "<td><input name=\"Darkest\" value=\"1\"></td>"\
               "<td>ед.</td>"\
            "</tr>"\
            "<tr>"\
                  "<td>Ярко(... - 1024): </td>"\
                 "<td><input name=\"Brightest\" value=\"1023\"></td>"\
               "<td>ед.</td>"\
            "</tr>"\
            "<tr>"\
                "<td>Освещенность: </td>"\
                 "<td><input disabled=\"disabled\" value=\"0\"></td>"\
               "<td>ед.</td>"\
            "</tr>"\
            "<tr>"\
                  "<td colspan=\"3\" height=\"30\"><b>Загрузка данных</b></td>"\
            "</tr>"\
            "<tr>"\
                "<td>Метеоданные: </td>"\
                 "<td><input name=\"updateTimer\" value=\"15\"></td>"\
                "<td> мин.</td>"\
            "</tr>"\
            "<tr>"\
                 "<td>ZIP код: </td>"\
                 "<td><input name=\"zipCode\" value=\"420202\"></td>"\
            "</tr>"\
            "<tr>"\
                "<td>Время: </td>"\
                 "<td><input name=\"updateClkTimer\" value=\"30\"></td>"\
                "<td> сек.</td>"\
            "</tr>"\
            "<tr>"\
                "<td>GMT: </td>"\
                 "<td><input name=\"GMT\" value=\"+3\"></td>"\
               "<td>час.</td>"\
            "</tr>"\
            "<tr>"\
                 "<td><a href=\"/network_settings\">Настройки сети</a></td>"\
                   "<td align=\"right\" colspan=\"2\"><input type=SUBMIT value=\"Сохранить\"></td>"\
            "</tr>"\
         "</table>"\
      "</form>"\
   "</body>"\
"</html>";

#endif
