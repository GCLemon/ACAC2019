-- ステージを記述する Lua スクリプト
-- コルーチンを利用している
function stage_movement()
    while true do
        for i = 1,120 do
            coroutine.yield();
        end
        position = {
            x = math.random(20, 620),
            y = math.random(20, 460)
        }
        add_enemy(position)
    end
end