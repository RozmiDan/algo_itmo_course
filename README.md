# algo_itmo_course
Курс Продвинутые АИСД ПИиКТ СППО 2024

Пройденные темы курса:
1. Корневая декомпозиция. Алгоритм Мо.
2. Дерево отрезков
3. AVL дерево.
4. Декартово дерево.
5. Префикс-функция. Z-функция.
6. Бор. Цифровой бор. Дерево ван Эмде Боаса.
7. LCA. Метод двоичного подъёма. LCA -> RMQ. Разреженная таблица.
8. Потоки в графах, алгоритм Форда-Фалкерсона.
 

# Проект для решения контестов по алгоритмам

- [Шаблон проекта для решения контестов по алгоритмам](#шаблон-проекта-для-решения-контестов-по-алгоритмам)
  - [Запуск проекта](#запуск-проекта)
  - [О тестирующей системе](#о-тестирующей-системе)
    - [Форматирование](#форматирование)
    - [Стиль кода](#стиль-кода)
    - [Параметры компиляции](#параметры-компиляции)
  - [Рабочее окружение](#рабочее-окружение)
  - [Полезные советы](#полезные-советы)

## Запуск проекта

Необходимо установить официальный пакет IDE [VSCode][1] от Microsoft. Также вам понадобится установленный [Docker][2].

Установите плагин [VSCode][1]: [ms-vscode-remote.remote-containers][3].

Нажать сочетание клавиш `CLTR + SHIFT + P`, в строке поиска ввести `Dev Containers: Reopen in Container` и нажать клавишу `Enter`.

Плагин [Clangd][3] предложит установить языковой сервер, соглашайтесь.

Запустить "отладку файла" можно, открыв его и нажав `F5`.

## О тестирующей системе

Мы используем платформу [Яндекс.Контест][4]. При отправке файла с исходным кодом  в несколько этапов происходит сборка исполняемого файла.

### Форматирование

Сперва будет проверено соответствие вашего кода принятому форматированию с помощью утилиты [clang-format][5]. При выявлении проблемы, решение упадет со статусом `CE`.

Код должен соответствовать файлу конфигурации [.clang-format](/.clang-format).

### Стиль кода

На данный момент у нас не получилось наладить запуск `clang-tidy` в Яндекс.Контесте, поэтому стиль кода не будет проверяться.

Однако, мы добавили конфигурационный файл [.clang-tidy](/.clang-tidy). Языковой сервер [Clangd][11] подхватит его и будет выдавать вам полезные замечания не только по стилю наименования переменных, но и по использованию некоторых лучших практик.

Если вы крайне не согласны с линтером, то можете подавить замечание при помощи комментария `// NOLINT`.

Мы будем рады, если вы будете придерживаться данного стиля и присылать нам код без замечаний, ведь так нам будет проще его проверять и оказывать техническую поддержку.

### Параметры компиляции

Ваше решение будет собрано со флагами компиляции `CXXFLAGS_RELEASE`, определение которых вы найдете в файле [ci/build.sh](ci/build.sh#L16).

Так как будет включен максимальный уровень оптимизаций, вы можете не бояться использовать zero-cost абстракции языка. Благодаря `-DNDEBUG` вы свободно можете размещать код для отладочной печати или проверки инвариантов между директивами `#ifdef` and `#ifndef`, а также использовать макрос `assert`.

На малых тестах будет запускаться решение собранное под санитайзерами.

```bash
CXXFLAGS_ASAN="$CXXFLAGS_RELEASE -fsanitize=address,undefined,leak"
```

Так что мы надеемся поймать основные ошибки при работе с памятью, особенно утечки памяти. Санитайзер будет ронять программу, из-за чего вердиктом тестовой системы будет `RE`.

## Рабочее окружение

Для решения задач мы рекомендуем использовать официальный пакет IDE [VSCode][1] cо следующими плагинами:

- Для поддержки языкового сервера Clangd: [llvm-vs-code-extensions.vscode-clangd][6]
- Для отладки кода: [ms-vscode.cpptools][7] и [vadimcn.vscode-lldb][9]
- Для написания Makeﬁle: [ms-vscode.makeﬁle-tools][8]

Собирать решение рекомендуется с заданными флагами при помощи компилятора [clang][10].

[Clangd][11] предоставит вам возможности форматирования кода, предоставит подсказки, навигацию, рефакторинги и другое. Однако вам следует отключить `C_Cpp.intelliSenseEngine` от `Microsoft`, чтобы он не конфликтовал с `clangd` - скорее всего, `VSCode` предложит вам это сделать.

Для комфортной работы можете описать простую систему сборки с использованием утилиты `Make` или `Shell` скриптов. Можно автоматизировать не тольку сборку исполняемого файла, но и запуск тестов, что позволит вам сэкономить время и нервы.

И обязательно настройте компиляцию файла и запуск отладчика по кнопке в `VSCode` - оно того стоит.

## Полезные советы

Если ваше решение не проходит по времени, попробуйте сгенерировать большой тест, запустить решение под профилировщиком и построить [Flame Graph][12] - он может помочь диагностировать проблему. Собирая решение со включенными оптимизациями, добавьте флаг `-fno-omit-frame-pointer`. Большой тест может быть удобно генерировать скриптом на [Python][13].

Если ваше решение выдает неверный ответ или падает на одном из тестов, можете попробовать применить [технику тестирования программы случайными данными][14]. Для этого можете редактировать код функции `main` так, чтобы при поданом аргументе `--testing` запускать тестирование, а иначе код, читающий данные из стандартного ввода.

Удачи!

[1]: https://code.visualstudio.com/
[2]: https://www.docker.com/
[3]: https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers
[4]: https://contest.yandex.ru/edu
[5]: https://clang.llvm.org/docs/ClangFormat.html
[6]: https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd
[7]: https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools
[8]: https://marketplace.visualstudio.com/items?itemName=ms-vscode.makeﬁle-tools
[9]: https://marketplace.visualstudio.com/items?itemName=vadimcn.vscode-lldb
[10]: https://clang.llvm.org/
[11]: https://clangd.llvm.org/
[12]: https://www.brendangregg.com/flamegraphs.html
[13]: https://www.python.org/
[14]: https://en.wikipedia.org/wiki/Random_testing

