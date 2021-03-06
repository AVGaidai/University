Лабораторная работа No 1

Кодирование целых чисел

Ставится задача кодирования последовательности целых неотрицательных чисел
двоичными разделимыми кодами. Максимальное значение целого числа не задается (хотя
нам достаточно будет рассмотреть только числа, укладывающиеся в тип int).

Существует и можно еще придумать множество схем построения таких кодов.
Заранее ни одной из них нельзя отдать предпочтение. В конкретной предметной области
можно экспериментально (или с теоретическим обоснованием) выбрать наилучшую
схему, дающую максимальное сжатие.

Рассмотрим следующий подход, порождающий цепочку кодов φ 0 , φ 1 , φ 2 и т.д.
Обозначим через bin( x ) двоичную запись числа x , а через bint( x ) – усеченную двоичную
запись без ведущей единицы.

<table>
 <tr>
  <td> x         </td>
  <td> bin( x )  </td>
  <td> bint( x ) </td>
 </tr>
 <tr>
  <td> 0         </td>
  <td> 0         </td>
  <td>           </td>
 </tr>
 <tr>
  <td> 1         </td>
  <td> 1         </td>
  <td>           </td>
 </tr>
 <tr>
  <td> 2         </td>
  <td> 10        </td>
  <td> 0         </td>
 </tr>
 <tr>
  <td> 3         </td>
  <td> 11        </td>
  <td> 1         </td>
 </tr>
 <tr>
  <td> 4         </td>
  <td> 100       </td>
  <td> 00        </td>
 </tr>
 <tr>
  <td> 5         </td>
  <td> 101       </td>
  <td> 01        </td>
 </tr>
 <tr>
  <td> 6         </td>
  <td> 110       </td>
  <td> 10        </td>
 </tr>
 <tr>
  <td> 7         </td>
  <td> 111       </td>
  <td> 11        </td>
 </tr>
 <tr>
  <td> 8           </td>
  <td> 1000        </td>
  <td> 000         </td>
 </tr>
 <tr>
  <td>    ...      </td>
  <td>    ...      </td>
  <td>    ...      </td>
 </tr>
</table>

Очевидно, что ни bin( x ) , ни bint( x ) не могут кодировать целые числа в
последовательности, т.к. код получается неразделимым.

Определим код φ 0 ( x ) следующим образом: записываем x нулей, за которыми
следует единица.

<table>
 <tr>
  <td> x         </td>
  <td> φ 0 ( x ) </td>
 </tr>
 <tr>
  <td> 0         </td>
  <td> 1         </td>
 </tr>
 <tr>
  <td> 1         </td>
  <td> 01        </td>
 </tr>
 <tr>
  <td> 2         </td>
  <td> 001       </td>
 </tr>
 <tr>
  <td> 3         </td>
  <td> 0001      </td>
 </tr>
 <tr>
  <td> 4         </td>
  <td> 00001     </td>
 </tr>
</table>


Данный код разделим (единица отделяет кодовые слова друг от друга). Длина кода
для числа x равна x + 1 .

Определим теперь более экономичный код φ 1 ( x ) . Идея состоит в том, чтобы
использовать двоичную запись числа x , предварительно указав ее длину с помощью
разделимого кода φ 0 ( x ) . Так как двоичная запись (кроме записи для нуля) всегда
начинается с единицы, то целесообразно использовать bint( x ) , а ведущую единицу
декодер может добавить автоматически. Правило построения кода можно формально
записать следующим образом: φ 1 ( x ) = φ 0 (| bin( x ) |) D bint( x ) , где значком D показана
конкатенация (сцепление) кодовых слов. Для нуля делается исключение: он кодируется
одной единицей.

<table>
 <tr>
  <td> x         </td>
  <td> φ 1 ( x ) </td>
 </tr>
 <tr>
  <td> 0         </td>
  <td> 1         </td>
 </tr>
 <tr>
  <td> 1         </td>
  <td> 01        </td>
 </tr>
 <tr>
  <td> 2         </td>
  <td> 001 0     </td>
 </tr>
 <tr>
  <td> 3         </td>
  <td> 001 1     </td>
 </tr>
 <tr>
  <td> 4         </td>
  <td> 0001 00   </td>
 </tr>
 <tr>
  <td> 5         </td>
  <td> 0001 01   </td>
 </tr>
 <tr>
  <td> 6         </td>
  <td> 0001 10   </td>
 </tr>
 <tr>
  <td> 7         </td>
  <td> 0001 11   </td>
 </tr>
 <tr>
  <td> 8           </td>
  <td> 00001 000   </td>
 </tr>
</table>


В примере для наглядности части кода отделены пробелом, на самом деле никаких
пробелов вставлять не нужно. Правило декодирования простое: считаем количество нулей
до первой единицы (обозначим его через k ); если k = 0 , то получаем x = 0 ; если k > 0 , то
берем k бит, включая первую единицу, и они дают нам двоичную запись числа x .

Легко видеть, что для больших x длина кода | φ 1 ( x ) | ≈ 2 log( x ) , т.е. код φ 1
значительно экономнее кода φ 0 .

Можно построить еще более экономный код для больших чисел, если кодировать
длину двоичного представления не кодом φ 0 , а кодом φ 1 . Таким образом получаем код
φ 2 ( x ) = φ 1 (| bin( x ) |) D bint( x ) , делая опять исключение для нуля.

<table>
 <tr>
  <td> x         </td>
  <td> φ 2 ( x ) </td>
 </tr>
 <tr>
  <td> 0         </td>
  <td> 1         </td>
 </tr>
 <tr>
  <td> 1         </td>
  <td> 01        </td>
 </tr>
 <tr>
  <td> 2         </td>
  <td> 001 0 0   </td>
 </tr>
 <tr>
  <td> 3         </td>
  <td> 001 0 1   </td>
 </tr>
 <tr>
  <td> 4         </td>
  <td> 001 1 00  </td>
 </tr>
 <tr>
  <td> 5         </td>
  <td> 001 1 01  </td>
 </tr>
 <tr>
  <td> 6         </td>
  <td> 001 1 10  </td>
 </tr>
 <tr>
  <td> 7         </td>
  <td> 001 1 11  </td>
 </tr>
 <tr>
  <td> 8           </td>
  <td> 0001 00 000 </td>
 </tr>
</table>

Для большиx чисел x справедливо приближенное равенство

| φ 2 ( x ) | ≈ 2 loglog( x ) + log( x ) .

Представленную схему можно продолжать до бесконечности, строя все более
экономные коды для больших чисел.

Задание на ЛР.
Запрограммировать коды φ 0 , φ 1 , φ 2 с соответствующими
декодерами. Проверить корректность их работы на тестовых последовательностях целых
чисел.

Аудиторное время на выполнение и защиту работы – 4 часа.

Дополнительную информацию по кодам целых чисел легко найти в Интернете, см.
гамма и дельта коды Элайеса (Elias gamma (delta) coding), код Левенштейна, коды
Голомба (Golomb coding).

