// 一回更新するたびに経験値が1ずつ増加する
++Experience;

// 内部情報の書き出し
Console.WriteLine("{0:0000} / {0:0000} / {0:00}", Experience, ExperienceThreshold, Level);

// 閾値を超えたらレベルアップ処理を行う
if(Experience >= ExperienceThreshold) LevelUp();