function movement()
    while true do
        for i = 1,8 do
            move(3, 0)
            coroutine.yield()
        end
        for i = 1,8 do
            move(0, 3)
            coroutine.yield()
        end
        for i = 1,8 do
            move(-3, 0)
            coroutine.yield()
        end
        for i = 1,8 do
            move(0, -3)
            coroutine.yield()
        end
    end
end