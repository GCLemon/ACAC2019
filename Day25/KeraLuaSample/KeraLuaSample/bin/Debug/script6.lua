-- コルーチンを用いた関数
function co()
    coroutine.yield("そこは広場だった")
    coroutine.yield("小さな滑り台があった")
    coroutine.yield("昔ここでよく遊んだことを思い出した")
end