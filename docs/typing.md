# Набор произвольного текста по нажатию на клавишу

Обычные системные средства не позволяют это сделать надежно, поэтому нужно сторонее приложение — Hammerspoon (да, без ложко-молотка тут не разберешься!).

## Установка и настройка

1. Скачать и установить приложение Hammerspoon (если есть homebrew, проще через него). Мак может поругаться на неподписанный апп, но он надежный, можно разрешить
2. Открыть настройки и дать нужные права
3. В меню выбрать Open config и вставить туда этот скрипт:
```lua
local app = hs.application
local window = hs.window
local key = hs.keycodes.map
local eventtap = hs.eventtap

local peppaText = "Давайте дружить и уважать друг друга"
local bebopText = "Ты идешь нахуй по причине конченный долбоеб"

hs.hotkey.bind({}, "F14", function()
    writeText(peppaText)
end)

hs.hotkey.bind({}, "F15", function()
    writeText(bebopText)
end)

function writeText(textToWrite)
    local targetApp = app.frontmostApplication()
    
    if not targetApp then return end
    local mainWin = window.find(targetApp:pid(), "Untitled")
    
    if not mainWin then
        mainWin = targetApp:mainWindow()
    end

    if mainWin then
        mainWin:focus()
        
        hs.timer.doAfter(0.1, function()
            local contents = hs.pasteboard.getContents()
            hs.pasteboard.setContents(textToWrite)
            hs.eventtap.event.newKeyEvent({"cmd"}, "v", true):post()
            hs.eventtap.event.newKeyEvent({"cmd"}, "v", false):post()
            hs.timer.doAfter(0.1, function()
                if contents then
                    hs.pasteboard.setContents(contents)
                end
            end)
        end)
    else
        hs.notify.new("Ошибка", nil, "Целевое окно не найдено"):show()
    end
end
```
4. В скрипте есть два сообщения — поменяйте их на свои. По-дефолту зашиты хоткеи F14 и F15 (да, такие существуют, до F20). Проще всего назначить их на клавиши в [VIA](via.md) (придется через макросы), но можно поменять тут — как больше нравится
5. В меню Hammerspoon выбрать Reload config и наслаждаться быстрыми ответами на сложные ситуации

