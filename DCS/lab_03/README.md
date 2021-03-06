# ЛАБОРАТОРНАЯ РАБОТА No 3

<h1>Базовые задания.</h1>

Разработать программу, реализующую модель работы склада, отвечающего за хранение и
продажу некоторого товара (одного). Склад содержит N помещений, каждый из которых может 
хранить определённое количество единиц товара. Поступающий товар помещается в одно из 
помещений специальным погрузчиком. За товаром прибило K покупателей, каждому из которых 
требуется по Lk единиц товара. Площадка перед складом мала и на ней может в один момент 
времени находиться либо погрузчик, либо один из покупателей. Если покупателям требуется 
больше товара, чем имеется на складе, то они ждут новых поступлений, периодически 
проверяя склад. Время работы склада ограниченно.

Основная нить (функция main) выполняет следующие действия:

o Формирует начальное заполнение склада (для каждого помещения случайным 
образом выбирается число из диапазона от 1 до 40);

o Обрабатывает опции командной строки, в которой должно быть указано сколько клиентов 
будет обслуживаться складом и в течении какого времени должен склад работать;

o Порождает заданное количество нитей, каждая из которых реализует алгоритм работы
покупателя. Каждому покупателю случайным образом назначается количество требуемых 
единиц продукции (число из диапазона от 1 до 1000).

o Настраивает таймер (alarm) таким образом, чтобы он сработал по окончании времени
работы склада;

o Запускает алгоритм работы погрузчика;

o После срабатывания таймера принудительно завершает все выполняющиеся нити (если таковые имеются).

o Завершает работу программы.

Алгоритм работы погрузчика.

o Пытается попасть на площадку перед складом;

o Как только попадет на площадку, ищет хотя бы один склад, в котором нет продукции, и
заполняет его максимально возможным образом;

o покидает площадку;

o «засыпает» на 5 секунд;

o Цикл повторяется до срабатывания таймера;

Алгоритм работы покупателя.
o Пытается попасть на площадку перед складом;

o Как только попадет на площадку, ищет хотя бы один склад, в котором есть продукция,
и забирает либо столько, сколько надо, либо всю продукцию;

o покидает площадку;

o «засыпает» на 5 секунд;

o Цикл повторяется до тех пор, пока покупателю нужна продукция;

Программа должна на экран выводить информацию о помещениях склада.


<h1>Основные задания.</h1>
Доработайте программу умножения матриц из лабораторной работы № 2 с наилучшим способом 
обхода оперативной памяти так, чтобы использовалось автоматическое распараллеливание
циклов for. Продемонстрируйте, что результат умножения матриц получился правильным. Оцените
получившееся ускорение выполнения программы.

<h1>Задания повышенной сложности.</h1>
Спроектируйте и разработайте параллельное приложение, реализующее игру «крестики-нолики».

Одна нить отвечает за интерактивное взаимодействие с пользователем. Ожидает ввод с 
клавиатуры определённых клавиш (остальные клавиши игнорируются). Если пользователь нажимает
клавишу S, то нить сообщает второй нити, что можно начать или прекратить «играть» (см. ниже). 
Если нажата клавиша T, то пользователю предлагается ввести целое число в диапазоне от 1 до 10. 
После ввода первая нить меняет значение общей переменной timeThink. Если пользователь нажимает 
клавишу A, то ему предлагается ввести целое число в диапазоне от 5 до 30, которое записывается 
в общую переменную timeRestart.

Вторая нить реализует имитацию игры «Крестики-нолики» между двумя игроками. Каждый
игрок «думает» в течение времени timeThink и делает свой ход случайным образом в любую свободную 
ячейку. Игра продолжается до тех пор, пока какой-то из игроков не выиграет или не будет
заполнено все поле. После окончания игры происходит перезапуск после ожидания timeRestart секунд.
