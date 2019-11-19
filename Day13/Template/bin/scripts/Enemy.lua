  
-- 敵の動きを記述する Lua スクリプト
-- コルーチンを利用している
function EnemyMovement()
    while true do
        for i = 1,30 do
            enemy.position.x = enemy.position.x + 5;
            coroutine.yield();
        end
        enemy:launch({x = 0, y = 3});
        for i = 1,30 do
            enemy.position.y = enemy.position.y + 5;
            coroutine.yield();
        end
        enemy:launch({x = 0, y = 3});
        for i = 1,30 do
            enemy.position.x = enemy.position.x - 5;
            coroutine.yield();
        end
        enemy:launch({x = 0, y = 3});
        for i = 1,30 do
            enemy.position.y = enemy.position.y - 5;
            coroutine.yield();
        end
        enemy:launch({x = 0, y = 3});
    end
end