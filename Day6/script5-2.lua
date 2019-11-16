-- C++ で設定されたテーブルの内容を出力する
print("string = " .. table.string)
print("number = " .. table.number)
if table.boolean then
    print("boolean = true")
else
    print("boolean = false")
end