-- 敵の動きを記述する Lua スクリプト
-- コルーチンを利用している
function enemy_movement()
    while true do
        for i = 1,30 do
            position.x = position.x + 5;
            coroutine.yield(position);
        end
        for i = 1,30 do
            position.y = position.y + 5;
            coroutine.yield(position);
        end
        for i = 1,30 do
            position.x = position.x - 5;
            coroutine.yield(position);
        end
        for i = 1,30 do
            position.y = position.y - 5;
            coroutine.yield(position);
        end
    end
end