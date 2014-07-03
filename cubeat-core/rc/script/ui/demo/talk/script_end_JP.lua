local ffi   = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'


--[[
[pos]
change text & panel position
example=> pos={x=700, y=300}

[board]
change panel texture
example=> board="area_rect"

[board_flip]
"H"  = flip panel Horizon
"V"  = flip panel Vertical
"HV" = flip panel Horizon+Vertical

[ch]
1 = left actor
2 = right actor

[effect_a]
show
hide
slide_in
slide_out
fade_in
fade_out
shake

[effect_w]
size_L
size_M
size_S
shake

[special]
cube: x1,y1,x2,y2,x3,y3,path,w,h,dur

--]]


--
local act1_act1_ = {
  { ch=1, img="char1_new/normal", text="なるほど、\nこれでキューブを消せるんだ"},
  { ch=1, img="char1_new/normal", text="何が知らないうちに\nすげぇー力が手に入れたぜ" },
  { ch=1, img="char1_new/normal", text="すげぇー名前つけなきゃ、\nキューブ…ビート…" },
  { ch=1, img="char1_new/glad", text="そうだ！\n「キュビート」にしようぜ！"},
  { ch=1, img="char1_new/glad",text="すげぇーおもしろい！" },
  { ch=1, img="char1_new/glad",text="みんなに見せつけようぜ！" },
  
  { ch=1, text="" }
}
local act1_act2_ = {
  { ch=1, img="char1_new/glad", text="楽勝楽勝～"},
  { ch=2, img="char2_new/sad", text="うぅ～アキ負けた", board_flip="HV"},
  { ch=1, img="char1_new/normal", text="でもこうすると、\nオレ以外にも、"},
  { ch=1, img="char1_new/normal", text="キュビート能力が\n手に入れたやつがいるぜ" },
  { ch=1, img="char1_new/glad", text="よぉし！\nじゃ最強のキュビート能力者\nオレはなる！"}, 
  
  { ch=1, text="" }
}
local act1_act3_ = {
  { ch=1, img="char1_new/glad", text="はッ！面白ぇ！"}, 
  { ch=2, img="char3_new/sad", text="…信じられないです…",board_flip="H", effect_w="size_M" , board="dialogue_bubble_1"}, 
  { ch=1, img="char1_new/glad", text="それじゃを調べてくれるか、\n他にキュビート能力を\n持ってるやつ？"},
  
  { ch=1, text="" }
}
local act1_act4_ = {
  { ch=1, img="char1_new/glad", text="やっぱキュビート持ってんじゃん！\nでもオレのほうが強い！"},
  { ch=2, img="char4_new/sad", text="ワタクシは負けでないわよ\nあなたがうるさくて\n集中できなかっただけよ…",board_flip="HV", pos={x=480, y=230}},
  { ch=1, img="char1_new/normal", text="それで次はだれにしよか…？" },
  { ch=1, img="char1_new/normal", text="な、お前ほかに誰が\nキュビート能力持ってるか\n知ってる？" },
  { ch=2, img="char4_new/anger", text="ワタクシは\nあなたの質問に\n答える必要ないわ！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=350, y=180}}, 
  
  { ch=1, text="" }
}
local act1_act5_ = {
  { ch=1, img="char1_new/glad", text="よっしゃ！勝ったぜ！" },
  { ch=2, img="char5_new/anger", text="やるじゃないか、野球くん" ,board_flip="HV"},
  { ch=2, img="char5_new/normal", text="でも、拙者に勝ったくらいで、\n最強になったわけじゃないぞ" },
  { ch=1, img="char1_new/anger", text="先輩、\nそれはどういう意味だぜ？" },
  { ch=2, img="char5_new/anger", text="最強の道の前に\n立ちはだかる最大の敵は他ならぬ" },
  { ch=2, img="char5_new/glad", text="お前自身だ！" },  
  { ch=1, img="char1_new/anger", text="オレ自身！？" },
  { ch=2, img="char5_new/glad", text="そうだ！若いの！\n戦い続けるんだ！" },  
  { ch=2, img="char5_new/glad", text="すべての障壁に挑め！\nすべての相手に挑め！" },  
  { ch=2, img="char5_new/glad", text="自分の限界に挑戦するんだ！\n夕日に向かって走ろう！" },   
  { ch=2, img="char5_new/glad", text="リュタの戦いはこれからだ" },   
  { ch=1, img="char1_new/glad", text="おおおおおおおおぉぉ！！", board="dialogue_bubble_2", effect_w="size_L" },
  { ch=2, img="char2_new/glad", text="わわぁ、リュタ熱血だね！" },
  { ch=2, img="char4_new/normal", text="何この少年マンガのテンプレエンド" },  
  { ch=2, img="char6_new/anger", text="いい気にならないで" },  
  { ch=2, img="char3_new/glad", text="まあ、\n物語は終わりが必要だからです" ,board_flip="V"},  
  { ch=1, img="char1_new/glad", text="夕日に向かって走るぜ！", board="dialogue_bubble_1", effect_w="size_M" },
  
  { ch=1, text="" }
}
local act1_act6_ = {
  { ch=1, img="char1_new/glad", text="ははッ！勝った勝った！"},
  { ch=2, img="char6_new/sad", text="……",board_flip="H", pos={x=300, y=140}},
  { ch=1, img="char1_new/normal", text="…本当にキューブになったぜ" },
  
  { ch=1, text="" }
}

local act2_act1_ = {
  { ch=1, img="char2_new/glad", text="アキが勝ったね",board_flip="V"},
  { ch=2, img="char1_new/sad", text="そんなのってないよ…" ,board_flip="HV"},
  { ch=1, img="char2_new/normal", text="でもこのパズルゲーム\nどうしたの？", board_flip="V"},
  { ch=2, img="char1_new/normal", text="これはキュビートってんだ、\n面白ぇだろ"},
  { ch=1, img="char2_new/normal", text="へぇ～名前もあるんだねぇ。\nアキ初めて聞いたね" },
  { ch=2, img="char1_new/glad", text="そりゃあオレが名付けだ！\nついさき！"},
  { ch=2, img="char1_new/anger", text="キューブについては\nどういう事か\nさっぱりわからねぇ！"},
  { ch=2, img="char1_new/anger", text="面白ければいいんだぜ！" },
  { ch=2, img="char1_new/sad", text="ただまさか最初にお前とやったら\n負けるとは…"},
  { ch=2, img="char1_new/sad", text="……"},
  { ch=1, img="char2_new/normal", text="あ、うつモードに入った" },
  { ch=1, img="char2_new/normal", text="学校内にこのキューブを\n知ってる人いないかなぁ？" },
  
  { ch=1, text="" }
}
local act2_act2_ = {
  { ch=1, img="char2_new/normal", text="あ、もう放課後だね"},
  
  { ch=1, text="" }
}
local act2_act3_ = {
  { ch=2, img="char3_new/sad", text="これはきっと何がの間違いです…", board="dialogue_bubble_1",board_flip="H"}, 
  { ch=1, img="char2_new/glad", text="イエイ～勝っちゃった～",board_flip="V"},
  { ch=1, img="char2_new/glad", text="アキ、時間制御以外" },
  { ch=1, img="char2_new/glad", text="もしかしたら\n「キュビートが上手」の能力が\nあるかもねぇ！" },
  
  { ch=1, text="" }
}
local act2_act4_ = {
  { ch=1, img="char2_new/glad", text="リアまで勝っちゃった！\nイエイ～イエイ～" ,board_flip="V"},
  { ch=2, img="char4_new/anger", text="そんなのじゃないわ！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=350, y=180}},
  { ch=2, img="char4_new/sad", text="ワ、ワタクシは…\nアナタはワタクシの友たちだから、\nわざと勝たせたのよ！",board_flip="HV",effect_w="size_M" , board="dialogue_bubble_1"},
  { ch=2, img="char4_new/sad", text="ワタクシはもう絵を描くに戻るわ、\n邪魔しないで頂戴！"},
  { ch=1, img="char2_new/normal", text="え～じゃあアキは\n他に遊んでくれる人探しにくる"  ,board_flip="V"},
  
  { ch=1, text="" }
}
local act2_act5_ = {
  { ch=1, img="char2_new/glad", text="アキすごいでしょ！\nこれでも鍛える必要あるの？",board_flip="V"},
  { ch=2, img="char5_new/sad", text="…どうやら拙者のほうが\n山で野人と鍛える必要があるだね…" ,board_flip="H"},
  
  { ch=1, text="" }
}
local act2_act6_ = {
  { ch=1, img="char2_new/glad", text="イエイ～全勝！" ,board_flip="V"},
  { ch=2, img="char6_new/sad", text="……" ,board_flip="H"},
  { ch=1, img="char2_new/normal", text="でもいろんなひとに聞いたんだけど\n結局誰もキュビートは\nなんだったのか知らないね" ,board_flip="V"},
  { ch=1, img="char2_new/normal", text="バイオレットは\n一番わかりそうなのに" },
  { ch=1, img="char2_new/sad", text="もしかしたらあたしが\n寝ぼけてるだけ\nかもしれないかな？",board_flip="V" },
  { ch=1, img="char2_new/normal", text="あ、そうだ！" ,board_flip="V"},
  { ch=1, img="char2_new/normal", text="きっとあれだ、\nドラマのベタな展開だね！" },
  { ch=1, img="char2_new/glad", text="あたしきっと夢を見てるんだ！" ,board_flip="V"},
  { ch=2, img="char1_new/glad", text="けっこうおもしろい夢だったね！" ,board_flip="V" ,effect_a="slide_in"},
  { ch=2, img="char4_new/normal", text="これでごまかすのは適当すぎるわ"  ,effect_a="slide_in"}, 
  { ch=2, img="char6_new/sad", text="ふんッ…",board_flip="V"  ,effect_a="slide_in"},
  { ch=2, img="char5_new/glad", text="夢の中でも鍛えるんだね！",board_flip="V"  ,effect_a="slide_in"},
  { ch=2, img="char3_new/anger", text="おいおい、\n夢オチは禁止されてるですよ！" ,board_flip="V" ,effect_a="slide_in"},
  { ch=1, img="char2_new/glad", text="起こさないでよぉ～！" },
  
  { ch=1, text="" }
}

local act3_act1_ = {
  { ch=2, img="char1_new/sad", text="そんな…",board_flip="HV" },
  { ch=1, img="char3_new/glad", text="下次に宣戦する時に、\nまず相手を知ることです！"},
  { ch=1, img="char3_new/glad", text="でも宣戦しなくでも\n僕はそうしますけど、クク" },
  { ch=1, text="" }
}

local act3_act2_ = {
  { ch=2, img="char2_new/sad", text="みんなアキをいじめるぅ～",board_flip="HV"},
  { ch=1, img="char3_new/glad", text="先リョタと言ったな、\n次のターゲット決定！"},
  { ch=1, text="" }
}

local act3_act3_ = {
  { ch=1, img="char3_new/glad", text="よし、次はキューブを\nコントロールできる人を\n調べてみます…"},
  { ch=1, text="" }
}

local act3_act4_ = {
  { ch=1, img="char3_new/glad", text="僕の推測通り、\nやっぱりキミはキューブを\n制御する能力を持ってますね"},
  { ch=1, img="char3_new/glad", text="ただ技術はまだまだですけど、\nクク"},
  { ch=2, img="char4_new/sad", text="…こんな賞賛\nわからないわ…",board_flip="HV"},
  { ch=1, img="char3_new/glad", text="次に行きましょう"},
  { ch=1, text="" }
}

local act3_act5_ = {
  { ch=1, img="char3_new/normal", text="どっちの頭が\n鍛錬する必要があるですか？"},
  { ch=2, img="char5_new/sad", text="う……" ,board_flip="H"},
  { ch=1, text="" }
}

local act3_act6_ = {
  { ch=1, img="char3_new/glad", text="いきなり仕掛けるとは\n血の気多すぎますよ、クク"},
  { ch=2, img="char6_new/sad", text="……",board_flip="H"},
  { ch=1, img="char3_new/normal", text="でも、\n君は他のキュビート能力者と訳が\n違いますね"},
  { ch=1, img="char3_new/normal", text="キュープアクセサリがないのに、\nキュビート能力がある"},
  { ch=1, img="char3_new/normal", text="これは\n研究のし甲斐がある対象ですね"},
  { ch=1, img="char3_new/normal", text="それとも、\n君は体のど．こ．か．に\n隠しているですか？クク",board_flip="V"},
  { ch=2, img="char1_new/glad", text="キュビートに隠された秘密、\nようやくその姿が明かされるのか！"},
  { ch=2, img="char2_new/glad", text="次回、「真相」"},
  { ch=1, img="char3_new/anger", text="君たちいきなり出てきて何を言ってるですか！"},
  { ch=2, img="char5_new/glad", text="鍛錬の道は終わる事なく、\nこのストーリーも同じだ！"},
  { ch=2, img="char4_new/glad", text="何を言ってるのか分らないが、\nでももう最後だから\nワタクシも出ないと"},  
  { ch=2, img="char6_new/sad", text="……",board_flip="HV"},
  { ch=1, img="char3_new/anger", text="待て、どうして僕のエンディングは\n「つづく」なんだ！"},
  
  { ch=1, text="" }
}

local act4_act1_ = {
  { ch=1, img="char4_new/glad", text="ワタクシに挑むなら、\nそんな程度じゃないでしょう？" },
  { ch=2, img="char1_new/sad", text="お前やっばキュビート能力者か…\nしかもつえぇ…" ,board_flip="HV"},
  { ch=2, img="char1_new/sad", text="ヒロ先輩しかオレと対抗できる\nと思ったぜ…" },
  { ch=1, img="char4_new/normal", text="ヒロ先輩？" },
  
  { ch=1, text="" }
}
local act4_act2_ = {
  { ch=1, img="char4_new/glad", text="どう、ワタクシの手捌きは？" },
  { ch=2, img="char2_new/sad", text="みょん～" ,board_flip="HV"},
  { ch=1, img="char4_new/normal", text="もう勝手にしなさい" },
  
  { ch=1, text="" }
}
local act4_act3_ = {
  { ch=1, img="char4_new/glad", text="どう？\n思い知りましたか？"},
  { ch=2, img="char3_new/sad", text="そんなバカな…",board_flip="H"}, 
  { ch=1, img="char4_new/glad", text="ちゃんとワタクシのことを\nみんなに知らせなさい！"},
  
  { ch=1, text="" }
}
local act4_act4_ = {
  { ch=1, img="char4_new/glad", text="オホホ、ワタクシの画筆\n思った以上に凄いかもしれないわ"},
  { ch=1, img="char4_new/glad", text="ならワタクシが\n世界を美しくして\n差上げましょう！" },
  
  { ch=1, text="" }
}
local act4_act5_ = {
  { ch=2, img="char5_new/sad", text="ス、ストレッチを忘れだ…" ,board_flip="H"},
  { ch=1, img="char4_new/glad", text="柔道をやめて、\n絵でも描けたら？" },
  { ch=1, img="char4_new/glad", text="でもワタクシは\n才能のない部員はお断りよ" },
  { ch=2, img="char1_new/glad", text="オレオレ、入部希望するぜ！" ,board_flip="V"},
  { ch=2, img="char2_new/glad", text="アキも入る！" },
  { ch=2, img="char6_new/normal", text="…今のは？" },
  { ch=2, img="char3_new/normal", text="何ですか、\nこの訳分らない展開は？" },
  { ch=2, img="char5_new/glad", text="待ちたまえ、諸君！\n美術部に入るとは何だね？" },
  { ch=2, img="char5_new/anger", text="いま大事なのは、\n我らでキュビートの謎を\n解けるではないかね" },
  { ch=1, img="char4_new/normal", text="だからワタクシは\nキュビートを知らないと言ったわ" },
  { ch=2, img="char5_new/anger", text="ならそのキュープを\n操縦する能力は何だね？" },
  { ch=1, img="char4_new/normal", text="別に能力ではないわ" },
  { ch=1, img="char4_new/glad", text="ワタクシの画筆は\nこの醜い世界を変えれる、\n当り前のことですわ" },
  
  { ch=1, text="" }
}
local act4_act6_ = {
  { ch=1, img="char4_new/glad", text="あら、\n貴方本当に運がいいわね！" },
  { ch=1, img="char4_new/glad", text="ワタクシついつい\n予想以上の力を発揮したわ"},
  { ch=2, img="char6_new/sad", text="……",board_flip="H"},
  { ch=1, img="char4_new/normal", text="貴方と対話するのは\n難しいですわね"},
  { ch=1, img="char4_new/glad", text="それともワタクシに感心して\n言葉も出なくなったの？"},
  
  { ch=1, text="" }
}

local act5_act1_ = {
  { ch=2 , img="char1_new/sad" ,text="さすが伝説のヒロ先輩だぜ…" ,board_flip="HV"},
  { ch=1 , img="char5_new/glad" ,text="勝敗無常、\n大事なのは自分を鍛える事だね！ ",board_flip="V" },
  
  { ch=1, text="" }
}
local act5_act2_ = {
  { ch=1 , img="char5_new/glad" ,text="どうだね！ ",board_flip="V" },
  { ch=2 , img="char2_new/normal" ,text="どう～面白いでしょう！ " ,board_flip="HV"},
  { ch=1 , img="char5_new/normal" ,text="なぜ負けでも嬉しそうだね？ " },
  { ch=2 , img="char2_new/glad" ,text="アキは楽しめればそれていいなの～ " },
  { ch=1 , img="char5_new/glad" ,text="確かにある武闘家が言った：\n「戦いの中に答えはある！」 " },
  { ch=2 , img="char2_new/normal" ,text="本当はこのゲームは\nただのゲームかもしれない、\n楽しむ以外の目的はないんだ" },
  { ch=2 , img="char1_new/glad" ,text="そうだぜ！\nキュビートは超面白いぜ！ " },
  { ch=2 , img="char4_new/glad" ,text="次やりたいならアポを取りなさい" },
  { ch=2 , img="char3_new/glad" ,text="まあこんな感じです、クク",board_flip="V" },
  { ch=2 , img="char6_new/normal" ,text="…あの…私…" ,board_flip="V"},
  { ch=1 , img="char5_new/glad" ,text="じゃ拙者も挑戦し続けて、\n自分の答えを探すのだ！" },
  
  { ch=1, text="" }
}
local act5_act3_ = {
  { ch=1 , img="char5_new/glad" ,text="ハッ！拙者の勝利だね！" ,board_flip="V"},
  { ch=2 , img="char3_new/sad" ,text="……" ,board_flip="H"},
  { ch=1 , img="char5_new/normal" ,text="先まで口数が多いなのに" },
  
  { ch=1, text="" }
}
local act5_act4_ = {
  { ch=1 , img="char5_new/glad" ,text="どうだ！ " ,board_flip="V"},
  { ch=2 , img="char4_new/sad" ,text="…二度と\nあんた達の出前を頼まないわ… " ,board_flip="HV"},
  { ch=1 , img="char5_new/normal" ,text="ここは何も聞き出せそうにないか、\n次は… " },
  
  { ch=1, text="" }
}
local act5_act5_ = {
  { ch=1, img="char5_new/glad" ,text="ふん！瓦割りと変わらんな！ ",board_flip="V" },
  { ch=1, img="char5_new/normal" ,text="しかしこのキュープは一体？\n調べる必要があるだね… " },

  { ch=1, text="" }
}
local act5_act6_ = {
  { ch=1, img="char5_new/normal" ,text="勝つには勝ったが、\nなぜ仕掛けたのも分らん\n何が納得いかんだね",board_flip="V"},
  { ch=2, img="char6_new/sad" ,text="……",board_flip="H" ,effect_a="slide_in"  },
  { ch=1, img="char5_new/normal" ,text="魔術ちゃんがこんなじゃ\n何も聞き出せそうにないだね、\nただ… " },
  { ch=1, img="char5_new/normal" ,text="キュビートの話になると\n仕掛けてくる、\nまるで何かを隠すような" },
  { ch=1, img="char5_new/glad" ,text="裏がありそうだね" },
  
  { ch=1, text="" }
}

local act6_act1_ = {
  { ch=2, img="char1_new/sad" , text="つえぇ…",board_flip="HV"},
  { ch=1, img="char6_new/glad" , text="…どう？\n(二度とキュープを\n触りたくないでしょう！) ",board_flip="V"},
  { ch=1, img="char6_new/anger" , text="二度と…\nキュビートを口にするな！"},
  { ch=2, img="char1_new/anger" , text="え？何で！\nこのゲーム面白いのに！"},
  { ch=1, img="char6_new/normal" , text="…！？\n(なに！？)"},
  { ch=2, img="char2_new/glad" , text="アキもそう思う！面白いよ！",effect_a="slide_in" },
  { ch=2, img="char4_new/sad" , text="ふん！\nワタクシはべ、別に思わないわ",effect_a="slide_in" },
  { ch=2, img="char4_new/normal" , text="たまたま、\nワタクシの才能を見せると思って、"},
  { ch=2, img="char4_new/glad" , text="すこし遊んでもいいだけよ"},
  { ch=2, img="char5_new/glad" , text="拙者はいかなる挑戦も\n拒まないだね！",effect_a="slide_in" },
  { ch=2, img="char3_new/glad" , text="ふふん、謎は解けました、\nキミは消したいですね\n自分うっかり生れた産物！",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char3_new/glad" , text="まだ間に合うと思いますか？\nクク"},
  { ch=2, img="char1_new/glad" , text="これからキュビートは\nヒットゲームにあるだぜ！",board_flip="V",effect_a="slide_in" },
  { ch=1, img="char6_new/anger" , text="…お前ら！\n(こいつら！)"},
  { ch=1, img="char6_new/glad" , text="…！\n(もうどうにでもなぁれ！)"},

  { ch=1, text="" }
}
local act6_act2_ = {
  { ch=2, img="char2_new/sad" , text="うう、負けた",board_flip="HV"},
  { ch=1, img="char6_new/glad" , text="へへ！\n(また一人やっつけた！) ",board_flip="V"},
  
  { ch=1, text="" }
}
local act6_act3_ = {
  { ch=1, img="char6_new/glad" , text="ヘイ！\n(甘く見ないでね！)",board_flip="V"},
  { ch=2, img="char3_new/sad" , text="そんなバカな…",board_flip="H"},
  { ch=1, img="char6_new/normal" , text="うん…\n(次は…) "},
  
  { ch=1, text="" }
}
local act6_act4_ = {
  { ch=1, img="char6_new/glad" , text="ヘイ！\n(勝ったね！)",board_flip="V"},
  { ch=2, img="char4_new/sad" , text="ワタクシの画面の方がいいのに…",board_flip="HV"},
  { ch=1, img="char6_new/normal" , text="おう\n(この子まだ状況が分らないね)"},
  
  { ch=1, text="" }
}
local act6_act5_ = {
  { ch=1, img="char6_new/glad" , text="はは！\n(どうだ！)",board_flip="V"},
  { ch=2, img="char5_new/sad" , text="まさか拙者の努力が\nまだ足りないのかね！？",board_flip="H"},

  { ch=1, text="" }
}
local act6_act6_ = {
  { ch=1, img="char6_new/normal" , text="(超能力が暴走して、\nキュープ達を\n作っちゃったのかな？) ",board_flip="V"},
  { ch=1, img="char6_new/normal" , text="(なんか学園内にキュープが\nいっぱい散らかっている…) "},
  { ch=1, img="char6_new/anger" , text="(こうなったら、\nあれをするしか…) "},
  
  { ch=1, text="" }
}

local rundown_ = {
  r1_1 = act1_act1_,
  r1_2 = act1_act2_,
  r1_3 = act1_act3_,
  r1_4 = act1_act4_,
  r1_5 = act1_act5_,
  r1_6 = act1_act6_,
  
  r2_1 = act2_act1_,
  r2_2 = act2_act2_,
  r2_3 = act2_act3_,
  r2_4 = act2_act4_,
  r2_5 = act2_act5_,
  r2_6 = act2_act6_,
  
  r3_1 = act3_act1_,
  r3_2 = act3_act2_,
  r3_3 = act3_act3_,
  r3_4 = act3_act4_,
  r3_5 = act3_act5_,
  r3_6 = act3_act6_,
  
  r4_1 = act4_act1_,
  r4_2 = act4_act2_,
  r4_3 = act4_act3_,
  r4_4 = act4_act4_,
  r4_5 = act4_act5_,
  r4_6 = act4_act6_,
  
  r5_1 = act5_act1_,
  r5_2 = act5_act2_,
  r5_3 = act5_act3_,
  r5_4 = act5_act4_,
  r5_5 = act5_act5_,
  r5_6 = act5_act6_,
  
  r6_1 = act6_act1_,
  r6_2 = act6_act2_,
  r6_3 = act6_act3_,
  r6_4 = act6_act4_,
  r6_5 = act6_act5_,
  r6_6 = act6_act6_,
}


local function get_rundown(ch1, ch2)
  local k = 'r'..tostring(ch1)..'_'..tostring(ch2)
  
  return rundown_[k]
end


return {
  get_rundown = get_rundown
}