local ffi   = require 'ffi'
local C    = ffi.C
local view = require 'rc/script/ui/view'


--[[

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

--]]


--
local act1_act1_ = {
  { ch=1, img="char1_new/glad", text="放課後だ放課後！" ,effect_a="slide_in" },
  { ch=1, img="char1_new/glad", text="今日はいい天気だ！\n熱い練習しようぜ！" },
  { ch=1, img="char1_new/normal", text="ん？ちょっ、何だ今の？"},
  { ch=1, img="char1_new/sad", text="いてててて…" , board="dialogue_bubble_2", pos={x=380, y=180}, effect_w="shake", special={id="cube", x1=500, y1=-50, x2=350, y2=150, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250}},
  { ch=1, img="char1_new/sad", text="何がに当たったような…", effect_w="size_M", board="dialogue_bubble_1" },
  { ch=1, img="char1_new/anger", text="おい！\nなんだこのキューブは！？\n全部打ち返してやるぜ！" },
  { ch=1, text="" }
}

local act1_act2_ = {
  { ch=1, img="char1_new/glad", text="おーい！アキー！\nまだ教室にいるか？",effect_a="slide_in" },
  { ch=1, img="char1_new/glad", text="お前、\n朝から放課後まで寝てる気か！" },
  { ch=2, img="char2_new/normal", text="……" , board="dialogue_bubble_1" ,board_flip="HV"},
  { ch=1, img="char1_new/glad", text="やっぱりいたか。\nもう十分に寝ただろ。\nあのさあのさ…" },
  { ch=2, img="char2_new/normal", text="……" },
  { ch=1, img="char1_new/normal", text="ん？聞こえねえのか？" },
  { ch=1, img="char1_new/anger", text="おーい！アキ！" ,pos={x=350, y=150}, board="dialogue_bubble_2", effect_w="shake"},
  { ch=2, img="char2_new/normal", text="えぇ？リュウタか…",effect_a="shake"},
  { ch=1, img="char1_new/normal", text="なにボーっとしてんだ？" , board="dialogue_bubble_1",effect_w="size_M" },
  { ch=2, img="char2_new/normal", text="なんだかアタマがクラクラする…"},
  { ch=2, img="char2_new/sad", text="何かがアタマに当たった感じ。"},
  { ch=2, img="char2_new/sad", text="アタマの中、きゅ～ぷでいっぱい。" },
  { ch=2, img="char2_new/anger", text="それにアキが強くイメージすると、\nきゅ～ぷたちは\n思い通りに消えるんだ。"},
  { ch=1, img="char1_new/anger", text="おいおい…\nまさかお前もキュビート能力を\n手に入れたのか？"},
  { ch=2, img="char2_new/normal", text="キュゥビえ？なにそれ？"},
  { ch=1, img="char1_new/anger", text="アキッ！勝負だぜ！" , board="dialogue_bubble_2", effect_w="shake", effect_a="shake"},
  { ch=2, img="char2_new/normal", text="ええぇ！？" },
  { ch=1, img="char1_new/anger", text="どっちがキュビートを\nうまく使えるか、\nはっきりさせようぜ。", board="dialogue_bubble_1", effect_w="size_M" },
  
  { ch=1, text="" }
}

local act1_act3_ = {
  { ch=1, img="char1_new/normal", text="お～。\nコンピュータ部の設備って、\nすごそうに見えるな。",effect_a="slide_in" },
  { ch=2, img="char3_new/normal", text="何か用ですか？" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char1_new/normal", text="あ、いや別に。\nただ一つ聞きたいんだけど…" },
  { ch=1, img="char1_new/normal", text="知ってるか？\n学校で、大量の未確認キューブが\n出現したんだ。" },
  { ch=2, img="char3_new/normal", text="ふんっ。\n僕の情報収集能力に\n挑戦したいのですか？"},
  { ch=1, img="char1_new/normal", text="それじゃ、知ってるんだな。\nキューブを操れる能力を\n持つやつがいるんだぜ。"},
  { ch=1, img="char1_new/glad", text="その能力は、\n「キュビート」っていうんだ。\nオレが名付けたんだぜ。"},
  { ch=2, img="char3_new/glad", text="「キュビート」ですか？",board_flip="V"},
  { ch=2, img="char3_new/glad", text="君は、他にも誰かいるかと\n聞きたいんだね。"},
  { ch=2, img="char3_new/glad", text="そのキュビートって能力を\n持つ者が。"},
  { ch=2, img="char3_new/glad", text="フフフ…クククク…"},
  { ch=2, img="char3_new/anger", text="僕も\n持ってるんですよォ！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=330, y=260}},
  
  { ch=1, text="" }
}

local act1_act4_ = {
  { ch=1, img="char1_new/normal", text="美術室…こりゃ壮観だ。",effect_a="slide_in" },
  { ch=1, img="char1_new/normal", text="ジークが、ここにキュビートを\n持ってるやつがいるって。\n本当か？" },
  { ch=2, img="char4_new/normal", text="そこ、何をしてるの！\n絵を描くのを邪魔しないで頂戴！",board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char1_new/normal", text="絵？\nこれ全部お前が描いたのか？" }, 
  { ch=1, img="char1_new/anger", text="きれい！\n本当すげーな！"},
  { ch=2, img="char4_new/glad", text="ふん！こ、こんな程度の賞賛で\nワタクシが喜ぶと思って？"}, 
  { ch=1, img="char1_new/glad", text="って、お前も\nキュビート能力持ってるんだろ！"},
  { ch=1, img="char1_new/glad", text="キューブを操って、\n並べて消す能力！"},
  { ch=2, img="char4_new/normal", text="何のこと言ってるのか、\nわからないわ。"},
  { ch=1, img="char1_new/sad", text="え？\nジークが教えてくれたのは、\n偽情報か？"},
  { ch=1, img="char1_new/sad", text="がっかりだぜ…"},
  { ch=2, img="char4_new/anger", text="無礼な！誰もワタクシ\nに失望できないわ！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=350, y=180}},
  
  { ch=1, text="" }
}

local act1_act5_ = {
  { ch=1, img="char1_new/normal", text="翔鯨道場…\nいつの間にかここに来ちまったぜ。",effect_a="slide_in" },
  { ch=1, img="char1_new/normal", text="伝説のヒロ先輩、\nいるかな？" },
  { ch=1, img="char1_new/glad", text="もし本当に伝説通りの強さなら、\n彼に勝ったら\nオレが最強ってことだ！"},
  { ch=2, img="char5_new/normal", text="ははは、野球少年！\n君も心身鍛錬しにきたんかね？" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char1_new/normal", text="心身鍛錬？"},
  { ch=1, img="char1_new/normal", text="違うよ。オレは、\nキューブを消せる能力を\n持ってるやつを探してるんだ。" },
  { ch=2, img="char5_new/glad", text="ははッ！\nそれならここで間違ってないぞ、\n野球少年！"},
  { ch=1, img="char1_new/anger", text="やっぱり！" },
  { ch=1, img="char1_new/anger", text="先輩！\nオレと勝負しようぜ！" },
  { ch=1, img="char1_new/anger", text="先輩に勝てば、\nオレが最強になれる！" },
  { ch=1, img="char1_new/normal", text="…そんな気がする。"},
  { ch=2, img="char5_new/normal", text="最強になりたいかね？"},
  { ch=2, img="char5_new/anger", text="野球少年、\n君は考えたことあるかね？\n最強とは何かと？"},
  { ch=2, img="char5_new/normal", text="最強になったら、\nその次に何がある？"},
  { ch=1, img="char1_new/anger", text="そりゃもちろん…", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=360, y=100} },
  { ch=1, img="char1_new/anger", text="そりゃ…" , board="dialogue_bubble_1", effect_w="size_M"},
  { ch=1, img="char1_new/normal", text="そうだな、考えたことないぜ。"},
  { ch=2, img="char5_new/anger", text="まず頭を使うんだ！\n若いの！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=380, y=80} },
  { ch=1, img="char1_new/sad", text="説教はオレに\n勝ってからにしてくれよ！"},
  { ch=2, img="char5_new/glad", text="なら拙者も手加減なしで行くぞ。" , board="dialogue_bubble_1", effect_w="size_M"},
  { ch=1, text="" }
}

local act1_act6_ = {
  { ch=1, img="char1_new/normal", text="キュビートって、\n不思議な力だな。\nなんだか魔術みたいだ。",effect_a="slide_in" },
  { ch=1, img="char1_new/normal", text="魔術部のやつらが、\n何か知ってるかもしれないな。"},
  { ch=1, img="char1_new/glad", text="ヤッホー！\n魔術部のみなさん、\nこんにちは！" },
  { ch=2, img="char6_new/normal", text="……。" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char1_new/normal", text="あの…こんにちは？"},
  { ch=2, img="char6_new/glad", text="……。"},
  { ch=1, img="char1_new/glad", text="おお！すげぇ！\nさすが魔術師、\n面白い挨拶だ。"},
  { ch=2, img="char6_new/normal", text="……うん。"},
  { ch=1, img="char1_new/normal", text="お前、\nホント魔術が得意だな。" },
  { ch=1, img="char1_new/normal", text="じゃあ、\nキューブを使った「魔術」も、\n問題なさそうだな？"},
  { ch=2, img="char6_new/anger", text="ふんっ！！"},
  { ch=1, img="char1_new/anger", text="ええ？いきなり勝負かよ！？" },
  
  { ch=1, text="" }
}

local act2_act1_ = {
  { ch=1, img="char2_new/normal", text="あ、リュタだ～。",effect_a="slide_in" },
  { ch=2, img="char1_new/glad", text="アキ、ちょうどいい、\n面白ぇもん見せてやるよ。"  ,board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char2_new/glad", text="わ～い本当？\n何かな、\nウサギみたいな猫かな？" ,board_flip="V"},
  { ch=2, img="char1_new/normal", text="全然違う。\nていうかそれ、\n面白いのか？"},
  { ch=1, img="char2_new/anger", text="あったら面白いのにぃ。"},
  { ch=2, img="char1_new/anger", text="犬とか猫とか置いといて！\nこれ試そうぜ！"},
  
  { ch=1, text="" }
}

local act2_act2_ = {
  { ch=1, img="char2_new/normal", text="今日はいい天気だねぇ～。"},
  { ch=1, img="char2_new/normal", text="なんか、一日ずぅーっと\n雲を見て過ごせる感じ。" },
  { ch=1, img="char2_new/normal", text="こうしてるだけで、\nすっごく幸せだよ。" },
  { ch=1, img="char2_new/sad", text="いたッ！" ,effect_w="shake", board="dialogue_bubble_2", pos={x=370, y=120}, special={id="cube", x1=500, y1=-50, x2=300, y2=150, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250}},
  { ch=1, img="char2_new/anger", text="もー！誰よ、\nあたしの頭をぶったのは！", effect_w="size_M", board="dialogue_bubble_1" ,board_flip="V" },
  { ch=1, img="char2_new/normal", text="え？誰もいない？",board_flip="V"},
  { ch=1, img="char2_new/normal", text="変だなぁ…でもなんか、\n目の前にキューブが\nいっぱいだねぇ…" },
   
  { ch=1, text="" }
}

local act2_act3_ = {
  { ch=1, img="char2_new/normal", text="ここは\nコンピューターがいっぱいだね。\nコンピューター部かな。" ,effect_a="slide_in" },
  { ch=1, img="char2_new/normal", text="パソコンってテレビと似てるねぇ。\nでもパソコンより、\nアキはテレビのほうが好き。"},
  { ch=1, img="char2_new/normal", text="だってテレビのほうが、\nアタマ使わなくっていいからねぇ。"},
  { ch=2, img="char3_new/normal", text="お…ちょうどよかった。\n直々に現れたのですね。",board_flip="HV" ,effect_a="slide_in"},  
  { ch=2, img="char3_new/glad", text="先ほどの対戦、\n見ましたよ、クク。",board_flip="V"},
  { ch=2, img="char3_new/glad", text="カウントダウン時間が長いとはね。"},
  { ch=2, img="char3_new/glad", text="どうやら君は、\n時間を制御できる特殊属性を\n持っているようです。" },
  { ch=1, img="char2_new/normal", text="へ？なに？\n時間を制御？\nアキが時間を制御できるの？"},
  { ch=1, img="char2_new/normal", text="そういえば、\nアキはいつも外を\nぼーっと見てると、"},
  { ch=1, img="char2_new/normal", text="時間がゆっくりになったような\n気がするの。"},
  { ch=1, img="char2_new/anger", text="そしてつい寝ちゃって、\n起きたらもう夜なんだよぉ。\n逆に時間がすっごく早くなる。" ,board_flip="V"},
  { ch=1, img="char2_new/glad", text="それがあなたの言う時間制御？\n特殊能力だったんだ、\n知らなかったねぇ。"},
  { ch=2, img="char3_new/normal", text="えッ…いや、\nそれはたぶん違うと思います…。",board_flip="V"},  
  { ch=1, img="char2_new/anger", text="なによ、あなたさっきアキに、\n時間制御能力が\nあると言ったのにぃ。"},
  { ch=2, img="char3_new/anger", text="話が通じない！", board="dialogue_bubble_2", effect_w="shake", effect_a="shake", pos={x=330, y=260}},
  
  { ch=1, text="" }
}

local act2_act4_ = {
  { ch=1, img="char2_new/normal", text="わぁ～。\nリアの絵、\n相変わらずきれいだねぇ。" ,effect_a="slide_in" },
  { ch=2, img="char4_new/glad", text="ん？アキか。",board_flip="HV" ,effect_a="slide_in"},
  { ch=2, img="char4_new/normal", text="絵画の素晴らしさが\nわかるような品性が、\nアナタにあるのか疑問だけど。"},
  { ch=2, img="char4_new/glad", text="ワタクシの絵は、\n誰も失望させたことないわ。"},
  { ch=1, img="char2_new/glad", text="へぇ～。\nリアすごぉ～い。",board_flip="V"},
  { ch=2, img="char4_new/normal", text="それで、どうなさったの？\nわざわざ部室まで会いにきて。"},
  { ch=1, img="char2_new/normal", text="それがね、なんかアキは\nすごくなった感じがするの。\n特殊能力があるみたい！" ,board_flip="V"},
  { ch=2, img="char4_new/glad", text="寝ても覚めても夢を見てる以外に、\n何の特殊能力があって？"},
  { ch=1, img="char2_new/glad", text="え、そんなのもあるの？\nアキ、また新しい特殊能力が\n増えちゃったぁ！" ,board_flip="V"},
  { ch=2, img="char4_new/normal", text="アナタがいつも白昼夢を見てる\nという意味よ。"},
  { ch=1, img="char2_new/anger", text="そんなことないよぉ、\nアキの能力すごいんだよ！\nほら見て！" },
  
  { ch=1, text="" }
}

local act2_act5_ = {
  { ch=1, img="char2_new/normal", text="柔道部って木が多いねぇ。\n森みたい。",effect_a="slide_in" },
  { ch=2, img="char5_new/glad", text="ハハハッ！アホ毛くん！\n君も心身鍛錬しに来たのかね？" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char2_new/glad", text="うわっ！\nジャングルの野人があらわれた！",board_flip="V"},
  { ch=2, img="char5_new/sad", text="ジャングルの…野人…",board_flip="V"},
  { ch=2, img="char5_new/normal", text="いいか、\n拙者は翔鯨道場師範、\nジャングルの野人なんかじゃない。" ,board_flip="V"},
  { ch=1, img="char2_new/anger", text="えぇ～つまんない～。\n野人があらわれたほうが\n面白いのにぃ。"},
  { ch=2, img="char5_new/anger", text="野人はもうどうでもいい！"},
  { ch=2, img="char5_new/glad", text="君がここに来た目的は？\nミッチリ鍛えたいのかね？"},
  { ch=1, img="char2_new/normal", text="鍛えるとか、\n疲れそうだねぇ。" ,board_flip="V"},
  { ch=1, img="char2_new/anger", text="アキ疲れるのイヤ～。" ,board_flip="V"},
  { ch=2, img="char5_new/anger", text="若者よ、\nそんな体たらくでどうする！\n戦いを覚悟するんだ！"},
  
  { ch=1, text="" }
}

local act2_act6_ = {
  { ch=1, img="char2_new/glad", text="魔術部だァ！\nアキも魔術だいすき！" ,board_flip="V",effect_a="slide_in" },
  { ch=2, img="char6_new/normal", text="……。" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char2_new/normal", text="あ、\nあなたが\n魔術部のバイオレットでしょ。" ,board_flip="V"},
  { ch=1, img="char2_new/normal", text="魔術がすごく上手って聞いたよ。\nアキにも見せてくれる？"},
  { ch=2, img="char6_new/glad", text="……うん。"},
  { ch=1, img="char2_new/glad", text="イエイ～！ハトだ！" ,board_flip="V"},
  { ch=1, img="char2_new/normal", text="実はアキ、\n猫のほうが好きなんだ。",board_flip="V"},
  { ch=2, img="char6_new/normal", text="……そう。"},
  { ch=1, img="char2_new/normal", text="そうだ、バイオレット。"},
  { ch=1, img="char2_new/normal", text="キューブを\n変化させるキュビートも、\n魔術の一種なの？"},
  { ch=2, img="char6_new/normal", text="……違う。"},
  { ch=1, img="char2_new/anger", text="ああ～残念。わたしも魔術が\nできるようになったのかと\n思ったのにぃ。",board_flip="V"},
  { ch=2, img="char6_new/anger", text="そんな簡単…じゃない！"},
  
  { ch=1, text="" }
}

local act3_act1_ = {
  { ch=1, img="char3_new/normal", text="ん？",effect_a="slide_in" },
  { ch=2, img="char1_new/glad", text="あはっ！\n最強のキュビート能力者に、\nオレはなる！" ,board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char3_new/normal", text="そこの赤毛くん。" },
  { ch=2, img="char1_new/normal", text="よう、何の用だ？"},
  { ch=1, img="char3_new/glad", text="そのキューブ型リストバンド…\n珍しいですね、クク。"},
  { ch=2, img="char1_new/glad", text="かっこいいだろ！\nでもキューブと言えば、\nもっといい物があるぜ…。"},
  { ch=1, img="char3_new/normal", text="キューブの制御能力ですね。"},
  { ch=2, img="char1_new/anger", text="キュビートだぜ！オレが名付だ！"},
  { ch=2, img="char1_new/normal", text="でもなんで分かった？"}, 
  { ch=1, img="char3_new/glad", text="いい名前ですね。\nなぜ分かったかは重要じゃない。\n重要なのは…。"},
  { ch=2, img="char1_new/glad", text="お前もキュビートできるのか！\nじゃ、勝負するぞ！"},
  { ch=1, img="char3_new/anger", text="望むところです！", effect_w="shake", effect_a="shake", board="dialogue_bubble_2", pos={x=350, y=260}},

  { ch=1, text="" }
}

local act3_act2_ = {
  { ch=1, img="char3_new/normal", text="情報によると、\nあの能天気なアキもキューブを\n制御出来るそうです。",effect_a="slide_in"},
  { ch=1, img="char3_new/glad", text="どうやら、頭の良さは\nキューブを制御できる条件\nではないようですね。"},
  { ch=1, img="char3_new/normal", text="ねえ、アキ。\nちょっと聞きたいことが\nあるんです。"},
  { ch=2, img="char2_new/normal", text="そぉ～？でもアキ、\n今アタマがクラクラするんだよぉ。",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char3_new/normal", text="キューブが頭に当たったん？\n大丈夫です、\n頭が悪くなる心配はないです。"},
  { ch=1, img="char3_new/normal", text="だってこれ以上バカになるのは、\n不可能です。"},
  { ch=2, img="char2_new/anger", text="え～ひどい！"},
  { ch=2, img="char2_new/normal", text="アキ、\n今特殊な能力が使えるんだよ！\nキューブを操れるの！"},
  { ch=1, img="char3_new/glad", text="クク、やっぱり！\n君もキューブを制御する能力が\nあるんですね。"},
  { ch=2, img="char2_new/anger", text="まさかリュウタと同じで、\nまたアキと対戦したいの？"},
  { ch=2, img="char2_new/glad", text="なんだかアキ、\nモテモテみたいだね！"},
  { ch=1, img="char3_new/normal", text="そうですね。"},
  { ch=1, img="char3_new/glad", text="いいカモですよ。"},
  
  { ch=1, text="" }
}

local act3_act3_ = {
  { ch=1, img="char3_new/glad", text="今日も何か起こってますかね？\nククク。"},
  { ch=1, img="char3_new/glad", text="僕が作った\n校内情報交換システムは、"},
  { ch=1, img="char3_new/glad", text="いつも面白い情報を\n持ってきてくれますね。"},
  { ch=1, img="char3_new/glad", text="どれどれ…"},
  { ch=1, img="char3_new/glad", text="「伝説によると、学校マスコット\nである飛行クジラの大好物は、\nポテチなんだ！」"},
  { ch=1, img="char3_new/glad", text="「実はこの学校の校長は、\n宇宙リトルブルーマン！」"},
  { ch=1, img="char3_new/anger", text="なぜこんな訳の分らない情報\nばっかりなんですか！", effect_a="shake", board="dialogue_bubble_2", pos={x=360, y=260}},
  { ch=1, img="char3_new/normal", text="うん？これは何ですか？", board="dialogue_bubble_1", effect_w="size_M"},
  { ch=1, img="char3_new/normal", text="「学校内に大量の\nミステリーキューブが発生…"},
  { ch=1, img="char3_new/normal", text="…このキューブの出処も用途も、\nいまだ不明…"},
  { ch=1, img="char3_new/normal", text="目撃者によると、一部の人間が\nキューブを使って対戦する\n能力を持っていると…」"},  
  { ch=1, img="char3_new/glad", text="面白い。\nキューブを操れるのですね？"},
  { ch=1, img="char3_new/glad", text="ちょうどいい。\n確かにいくつか同じような\nキューブを見ましたね。"},
  { ch=1, img="char3_new/glad", text="早速試してみましょう、\nクク。"},
  { ch=1, text="" }
}

local act3_act4_ = {
  { ch=2, img="char4_new/normal", text="あら。コンピューター部が\nワタクシに何か用？",board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char3_new/glad", text="そうですね。ここがちょっと、\n普通じゃないと思っただけです、\nクク。",effect_a="slide_in"},
  { ch=2, img="char4_new/glad", text="あら？\n今更ワタクシの\n普通じゃない技法に気付いたの？"},
  { ch=1, img="char3_new/normal", text="いや。そういうことを\n言ってるんじゃないです。"},
  { ch=2, img="char4_new/glad", text="ふふん、確かにワタクシの絵も、\n普通の価格で\n買えるものじゃないわ。"},
  { ch=1, img="char3_new/normal", text="ちょっと、\n話を聞いてください。"},
  { ch=2, img="char4_new/glad", text="普通の賞賛では、\nワタクシは受付けないわ。"},
  { ch=1, img="char3_new/anger", text="話が通じない！" ,effect_w="shake", effect_a="shake",board="dialogue_bubble_2", pos={x=360, y=260}},

  { ch=1, text="" }
}

local act3_act5_ = {
  { ch=1, img="char3_new/normal", text="そろそろ、\nキュビート能力の獲得条件が\n見えてきました。",effect_a="slide_in" },
  { ch=1, img="char3_new/normal", text="リアとアキの髪飾り。\nリュウタのリストバンド…" },
  { ch=1, img="char3_new/normal", text="キューブ型アクセサリを着けて、\nキューブに触ると…" },
  { ch=1, img="char3_new/glad", text="キュビート能力が手に入る！"},
  { ch=1, img="char3_new/glad", text="次の能力者は、\nこの、ベルトに\nキューブがついた男ですね。" },
  { ch=2, img="char5_new/normal", text="おう？パソコン少年、\n君も心身鍛錬したいかね？",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char3_new/normal", text="興味ありません。"},
  { ch=2, img="char5_new/glad", text="遠慮するな！\nずっとパソコンいじってて、\n運動不足なんだろう！"},
  { ch=1, img="char3_new/normal", text="脳筋の人に\n説教されたくありません。" },
  { ch=2, img="char5_new/glad", text="パソコンより大事なのは、\n君の頭だ！\nそしてその頭と直結する体だ！"},
  { ch=1, img="char3_new/anger", text="うるさいですね！" ,effect_w="shake", effect_a="shake",board="dialogue_bubble_2", pos={x=350, y=260}},

  { ch=1, text="" }
}

local act3_act6_ = {
  { ch=1, img="char3_new/normal", text="最新の情報では、\n魔術部にもキュビートが\n達者な者がいると。",effect_a="slide_in" },
  { ch=1, img="char3_new/normal", text="確かに魔術部に、\nマジックは上手いが口ベタな、\n女マジシャンがいますね…。"},
  { ch=2, img="char6_new/glad", text="……なに。",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char3_new/glad", text="どうやら僕が探している人物は、\n君のようですね。"},
  { ch=2, img="char6_new/normal", text="……？"},
  { ch=1, img="char3_new/normal", text="待てよ？\n君はキューブ型のアクセサリを\n着けていませんね。"},
  { ch=1, img="char3_new/normal", text="キュビート能力者\nではありませんか…？"},
  { ch=2, img="char6_new/anger", text="ふんッ！"},
  
  { ch=1, text="" }
}

local act4_act1_ = {
  { ch=1, img="char4_new/normal", text="あら？",effect_a="slide_in"},
  { ch=2, img="char1_new/glad", text="見つけたぞ～！\nあんたを探してたんだぜ！",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char4_new/glad", text="ふふ。\nどうやらワタクシの名は、\n確実に広まっているようね！"},
  { ch=2, img="char1_new/anger", text="オレは、キュビート能力者を\n探してるんだ！"},
  { ch=2, img="char1_new/glad", text="あんたも持ってると聞いたんだ。\n早速対戦しようぜ！"},
  { ch=1, img="char4_new/normal", text="キュビート？何ですの、それ？\nワタクシ知らないわ。"},
  { ch=2, img="char1_new/normal", text="そっか。\nさっき名付けたばっかり\nだからな。"},
  { ch=1, img="char4_new/normal", text="なんだか適当ですわね。\n興味ありませんわ。"},
  { ch=2, img="char1_new/anger", text="じゃ、\n適当にオレと勝負しようぜ！"},
  
  { ch=1, text="" }
}

local act4_act2_ = {
  { ch=1, img="char4_new/normal", text="まだ教室にいますの、アキ？\nもう放課後ですのよ。",effect_a="slide_in"},
  { ch=2, img="char2_new/normal", text="あ…",board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char4_new/normal", text="ボーっとしてないで、ワタクシに\n新たに目覚めた能力を、見よ！"},
  { ch=2, img="char2_new/glad", text="みょん？\nなんかかわいいね！"},
  { ch=1, img="char4_new/normal", text="みょんじゃなくて、見よ、ですわ。"},
  { ch=2, img="char2_new/glad", text="みょん～みょん～\nリアかわいい～。"}  ,
  { ch=1, img="char4_new/anger", text="いい加減、まじめにやりなさい！"} ,

  { ch=1, text="" }
}


local act4_act3_ = {
  { ch=1, img="char4_new/normal", text="コンピュター部には、\n学校情報ネットが\nあるのだそうですわ。",effect_a="slide_in"},
  { ch=1, img="char4_new/glad", text="もっとワタクシの存在を、\n大衆に知らせる時が来たわ。"},
  { ch=2, img="char3_new/normal", text="何か用ですか？",board_flip="HV" ,effect_a="slide_in"},
  { ch=1, img="char4_new/glad", text="ワタクシが訪問するなんて、\nアナタ運が良くってよ。"},
  { ch=2, img="char3_new/normal", text="あ、そう。"},  
  { ch=1, img="char4_new/normal", text="アナタの部屋、\n全然美しくないわ。"},
  { ch=1, img="char4_new/normal", text="ワタクシの絵を掛けて\nあげましょうか？"},
  { ch=1, img="char4_new/glad", text="勿論タダではありませんけど。\nオホホ。"},
  { ch=2, img="char3_new/normal", text="一体何の用ですか？"},
  { ch=1, img="char4_new/normal", text="ワタクシのことを知らないなんて。"},
  { ch=2, img="char3_new/glad", text="無駄な情報は集めないんです。",board_flip="V" },
  { ch=1, img="char4_new/anger", text="失礼な！" ,effect_w="shake", effect_a="shake",board="dialogue_bubble_2", pos={x=300, y=100}},
  
  { ch=1, text="" }
}

local act4_act4_ = {
  { ch=1, img="char4_new/normal", text="よし、これで絵が完成ですわ。",effect_a="slide_in"},
  { ch=1, img="char4_new/glad", text="まさに完璧ね。ワタクシの絵。"},
  { ch=1, img="char4_new/sad", text="ヒイ！",effect_w="shake", special={id="cube", x1=500, y1=-50, x2=270, y2=170, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250},board="dialogue_bubble_2",board_flip="V", pos={x=300, y=100} },
  { ch=1, img="char4_new/anger", text="何なの、このキューブは？",effect_w="size_M",board="dialogue_bubble_1",board_flip="V"},

  { ch=1, text="" }
}

local act4_act5_ = {
  { ch=1, img="char4_new/glad", text="喜びなさい、柔道部部員よ！\nワタクシが参りましたわよ！" ,effect_a="slide_in"},
  { ch=2, img="char5_new/normal", text="お？美術少女、\n君も心身鍛錬しに来たのかね？" ,board_flip="HV" ,effect_a="slide_in"},
  { ch=2, img="char5_new/anger", text="だが残念！\n私は今、ある難題の探求で\n手が離せないんだ。"},
  { ch=1, img="char4_new/anger", text="ワタクシが自ら訪問したのが、\nどんなに光栄なことかお分かり？"},
  { ch=1, img="char4_new/anger", text="誰もワタクシを拒絶出来ないわ！"},
  { ch=2, img="char5_new/normal", text="こりゃ失敬。"},
  { ch=2, img="char5_new/anger", text="君は、学校内に突然現れた\nミステリーキューブを\n知ってるかね？"},
  { ch=2, img="char5_new/anger", text="私は今、\nその原因を追っているんだよ。"},
  { ch=2, img="char5_new/normal", text="ふむ…私の予感だが…"},
  { ch=2, img="char5_new/glad", text="こんな時に訪ねて来た君にも、\n例の能力があるんではないか？" },
  { ch=1, img="char4_new/glad", text="あら、物分りがよろしいこと。\n後悔するわよ。"},
  
  { ch=1, text="" }
}

local act4_act6_ = {
  { ch=1, img="char4_new/normal", text="魔術部、ね。",effect_a="slide_in" },
  { ch=2, img="char6_new/glad", text="ハイ…。",board_flip="HV"},
  { ch=1, img="char4_new/normal", text="わざわざ迎えにこなくてもいいわ。\nちょっと通っただけよ。"},
  { ch=2, img="char6_new/normal", text="……。"},
  { ch=1, img="char4_new/glad", text="アナタは運がよろしいこと。\nワタクシ今日は、\n良い気分だわ。"},
  { ch=1, img="char4_new/glad", text="せっかくですから、\nワタクシの卓越した筆さばきを\nお見せしましょう！"},
  { ch=2, img="char6_new/anger", text="ふんッ！"},
  
  { ch=1, text="" }
}

local act5_act1_ = {
  { ch=2, img="char1_new/glad" ,text="みんな～\nキュビートやらない？" ,board_flip="HV",effect_a="slide_in"},
  { ch=1, img="char5_new/normal" ,text="キュビート？\nおい！野球少年！\nそのキュビートとは？",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char1_new/normal" ,text="超面白くて、不思議な力だ！" },
  { ch=2, img="char1_new/glad" ,text="学校内に発生したキューブを\n消せるんだぜ！" },
  { ch=1, img="char5_new/glad" ,text="どうやら野球少年も\n同じ目に遭ったようだな…" },
  { ch=2, img="char1_new/anger" ,text="え！あんた、\n伝説のヒロ先輩じゃねぇ？" },
  { ch=2, img="char1_new/glad" ,text="先輩もキュビート能力持ってる？\n一戦やらないか？" },
  { ch=1, img="char5_new/normal" ,text="その前に聞きたいんだが、\n野球少年はどうやって\n能力を手に入れた？" },
  { ch=2, img="char1_new/glad" ,text="突然使えたんだよ！\nすげーだろ！" },
  { ch=1, img="char5_new/normal" ,text="野球少年は、\nこの能力について、\nおかしいと思わないかね？" },
  { ch=2, img="char1_new/normal" ,text="思わない。" },
  { ch=1, img="char5_new/anger" ,text="頭を使いたまえ、若いの！" },
  { ch=1, img="char5_new/glad" ,text="力の原因を考えるのだ！\n力を持つ者の責任を考えよ！" },
  { ch=2, img="char1_new/anger" ,text="あ、分かった！" },
  { ch=2, img="char1_new/glad" ,text="たぶんオレは天才だからだな！" },
  { ch=1, img="char5_new/normal" ,text="野球少年…\n自信があるのは悪じゃない。"},
  { ch=1, img="char5_new/anger" ,text="だが、\n力と相応の謙虚さを覚えるべきだ！" },
  
  { ch=1, text="" }
}

local act5_act2_ = {
  { ch=1, img="char5_new/normal" ,text="手掛かりはあるが、\n謎はまだ解けないな。" ,board_flip="V",effect_a="slide_in" },
  { ch=2, img="char2_new/normal" ,text="あ、こんにちは～。\nキュビートやらない？" ,board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char5_new/normal" ,text="私の前に現れる者が、\n全員キュビート能力者とはね。\nもう驚かないが。" },
  { ch=1, img="char5_new/glad" ,text="ハッ！\nその前にアホ毛ちゃんに尋ねたい！" },
  { ch=1, img="char5_new/anger" ,text="君がみんなと\nキュビート対戦をする目的は\n何だね？" },
  { ch=2, img="char2_new/normal" ,text="う～ん。\n考えたことないな～。\nうんとね～…" },
  { ch=2, img="char2_new/glad" ,text="7回勝利すると、\n神様を召喚できて…" },
  { ch=2, img="char2_new/glad" ,text="それで願いを\n一つ叶えるためかな？" },
  { ch=1, img="char5_new/anger" ,text="このゲームにそんな要素はない！\nドラゴンなんとかじゃあるまいし！" },
  { ch=2, img="char2_new/sad" ,text="じゃ、\nこのゲームをやった人は、\n勝ち続けないと死ぬとか。" },
  { ch=1, img="char5_new/sad" ,text="恐ろしすぎるじゃないか！\n呪われたボードゲームか！？" ,board_flip="V"},
  { ch=2, img="char2_new/anger" ,text="ヒロ先輩細かすぎる～。" },
  { ch=1, img="char5_new/anger" ,text="アホ毛ちゃんは\n少し頭を使うべきだ！",board_flip="V" },
  { ch=2, img="char2_new/anger" ,text="じゃあ、\nあたしのすごさを見せてあげる！" },
  
  { ch=1, text="" }
}

local act5_act3_ = {
  { ch=1, img="char5_new/glad" ,text="ハッ！パソコン少年！\n少し尋ねたい事があるのだが！",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char3_new/normal" ,text="何ですか？\n身体を鍛えるのには、\n興味無いんですけど。",board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char5_new/normal" ,text="誤解するな。\n今回来たのは、情報通の君に\n聞きたい事があってのことだ。" },
  { ch=1, img="char5_new/normal" ,text="キュビートの件、\n君も情報を掴んでいるんだろう。" },
  { ch=2, img="char3_new/normal" ,text="脳筋で時代遅れのあなたも、\nキュビートに\n興味があるんですか？" },
  { ch=2, img="char3_new/glad" ,text="クク、その通りです。",board_flip="V" },
  { ch=1, img="char5_new/glad" ,text="未知への探求心も、\n人が生まれ持った、\n冒険への欲望だ！" },
  { ch=2, img="char3_new/normal" ,text="それで、何が知りたいんですか？" ,board_flip="V"},
  { ch=1, img="char5_new/normal" ,text="まずは…なぜ同じ色のキューブが\n並ぶと消えるのか？" },
  { ch=2, img="char3_new/glad" ,text="いい質問です。\n実はそれは、「普遍的無意識」が\n原因です。",board_flip="V" },
  { ch=1, img="char5_new/anger" ,text="何だね、それは？" },
  { ch=2, img="char3_new/glad" ,text="簡単に言うと、全ての人間の、\n無意識の深層に存在する\n共通の考えです。" },
  { ch=2, img="char3_new/glad" ,text="あるルールが、\n誰もが知っている状態になったら、\nそれが常識に変わる。" },
  { ch=1, img="char5_new/sad" ,text="うむ…もう少し簡単に言うと？" ,board_flip="V"},
  { ch=2, img="char3_new/glad" ,text="最初はあるパズルゲームで、\n同じ色のブロックが並ぶと消える\nというルールがありました。" },
  { ch=2, img="char3_new/glad" ,text="その設定をみんなが受け入れた。\nそして常識になった。\nそんな感じですね。" },
  { ch=1, img="char5_new/normal" ,text="よく考えもせずに、\nそのまま流されるのか？",board_flip="V" },
  { ch=1, img="char5_new/anger" ,text="私は少し納得いかんな。" },
  { ch=2, img="char3_new/normal" ,text="まあ、どうでもいいです。\nお喋りはここまで。\n次にやる事は分かりますね。" ,board_flip="V"},
  
  { ch=1, text="" }
}

local act5_act4_ = {
  { ch=1, img="char5_new/normal" ,text="キューブを消す要素は、色か…" ,board_flip="V",effect_a="slide_in" },
  { ch=1, img="char5_new/normal" ,text="それなら、\n色の専門家に聞くのが一番だな。" },
  { ch=1, img="char5_new/glad" ,text="ハッ！美術少女！\n少し尋ねたい事があるんだが！" },
  { ch=2, img="char4_new/normal" ,text="出前なんで頼んでないわよ。",board_flip="HV",effect_a="slide_in"  },
  { ch=1, img="char5_new/sad" ,text="…私がラーメン屋か寿司屋に\n見えるかね？" ,board_flip="V"},
  { ch=1, img="char5_new/normal" ,text="いやいや、私が来たのは、\n尋ねたい事があってな。" ,board_flip="V"},
  { ch=2, img="char4_new/glad" ,text="あら、\nワタクシに取材ですか？" },
  { ch=2, img="char4_new/normal" ,text="取材ならアポを取るのが基本よ？\nワタクシも暇じゃなくってよ。" },
  { ch=1, img="char5_new/anger" ,text="突然来たのは詫びよう。\nだがこの件で、\n君の力が必要でね！" },
  { ch=1, img="char5_new/anger" ,text="美術少女君は、\n学校内に大量のキューブが\n発生したことを知ってるかね？" },
  { ch=2, img="char4_new/glad" ,text="知ってるも何も、\nワタクシはキューブの制御も\nできてよ！" },
  { ch=1, img="char5_new/glad" ,text="やはりそうか！なら美術少女よ。\n君はこの現象をどう思うかね？" },
  { ch=2, img="char4_new/normal" ,text="どう思う？" },
  { ch=2, img="char4_new/glad" ,text="ワタクシの筆は世界を変える。\n当り前ですわよ？" },
  { ch=1, img="char5_new/anger" ,text="「当り前」で片づけては、\n物事を探求できないだろう。" },
  { ch=2, img="char4_new/anger" ,text="では、ワタクシの勝利が\n当り前だってこと、\n確かめてはどう？" },
  
  { ch=1, text="" }
}

local act5_act5_ = {
  { ch=1, img="char5_new/anger",text="喝！ハァッ！",board_flip="V" ,effect_a="slide_in" },
  { ch=1, img="char5_new/glad",text="わが技を見よ！！"},
  { ch=1, img="char5_new/sad",text="なん…！",board_flip="V",effect_w="shake", special={id="cube", x1=500, y1=-50, x2=300, y2=400, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250} },
  { ch=1, img="char5_new/sad",text="これは…ノーカウント…",effect_w="size_M"},
  { ch=1, img="char5_new/anger",text="私を邪魔したのは誰だ？\n何だ、このキューブの山？",board_flip="V" },
  { ch=1, img="char5_new/glad",text="よかろう、\n私の技を受けるがいい！！"},

  { ch=1, text="" }
}

local act5_act6_ = {
  { ch=1, img="char5_new/normal" ,text="普遍的無意識とか何とか言ったな…",board_flip="V" ,effect_a="slide_in" },
  { ch=1, img="char5_new/normal", text="ここは\n魔術の専門家の意見を聞こう。" },
  { ch=1, img="char5_new/glad" ,text="ハッ！魔術少女！\n少し尋ねたい事があるんだが！" },
  { ch=2, img="char6_new/normal" ,text="…ん？" ,board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char5_new/normal" ,text="君は、突然発生したキューブと、\nそれを制御するキュビート能力を\n知ってるかね？" },
  { ch=2, img="char6_new/anger" ,text="…ふん！" },
  { ch=1, img="char5_new/anger" ,text="殺気！？\n私が気分を害したかね？" },
  { ch=2, img="char6_new/anger" ,text="…だ…ま…れ…！" },
  
  { ch=1, text="" }
}

local act6_act1_ = {
  { ch=1, img="char6_new/normal" , text="うん…。\n(大体片付けたかな…)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char1_new/glad" , text="ヒャッホー！\nこの能力、面白れぇ！",board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char6_new/anger" , text="おい…お前！\n(次の目標決まり！)"},
  { ch=2, img="char1_new/normal" , text="ん？オレか？"},
  { ch=2, img="char1_new/anger" , text="まさか…\nキュビート能力者同士の\n呼び掛けか！？"},
  { ch=1, img="char6_new/normal" , text="…は？\n(どいつもこいつもバカばっか)"},
  { ch=2, img="char1_new/glad" , text="キューブを制御する能力の名前！"},
  { ch=2, img="char1_new/glad" , text="キューブとビットを\n掛けてキュビート！\nオレがさっき名付けた！"},
  { ch=1, img="char6_new/anger" , text="ふん！\n(訳が分からないわ！)"},

  { ch=1, text="" }
}

local act6_act2_ = {
  { ch=1, img="char6_new/normal" , text="…。\n(教室の方にもキューブが\n飛んできたようね)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char2_new/normal" , text="あ、魔術部の\nバイオレットちゃんだよね？\nかっこいいね。",board_flip="HV",effect_a="slide_in" },
  { ch=1, img="char6_new/glad" , text="…うん。\n(すこし見せちゃおう。)"},
  { ch=2, img="char2_new/glad" , text="わ～お！"},
  { ch=1, img="char6_new/normal" , text="…別に。\n(超能力に頼りたくないな…)"},
  { ch=2, img="char2_new/normal" , text="あ、そうだ。\n実はアキも魔術できるんだよ。"},
  { ch=2, img="char2_new/normal" , text="キューブの魔術だよ。\nついさっきできるように\nなったんだ。"},
  { ch=1, img="char6_new/anger" , text="ふん！(ボロボロに負かして、\n二度とキューブしたくないように\nしてやる！)"},

  { ch=1, text="" }
}

local act6_act3_ = {
  { ch=2, img="char3_new/glad" , text="…キュビート？\n面白い、\nキューブを制御するんですね？",board_flip="H",effect_a="slide_in" },
  { ch=1, img="char6_new/anger" , text="ふん！\n(この人はもう何かを\n掴んでるみたい！)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char3_new/normal" , text="おや？\nどうやら招かざる客ですね？",board_flip="V"},
  { ch=2, img="char3_new/glad" , text="いや、\n実はちょうどよかったんですけど。",board_flip="V"},
  { ch=1, img="char6_new/anger" , text="やっぱり…！\n(この人もやっつけないと！)"},

  { ch=1, text="" }
}

local act6_act4_ = {
  { ch=1, img="char6_new/normal" , text="…。\n(まずは\n近くの部室からチェックしよ。)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char4_new/glad" , text="あら、今日の筆さばきは\nいつもより好調ですわ。",board_flip="HV",effect_a="slide_in" },
  { ch=2, img="char4_new/normal" , text="ん？アナタは…魔術部の？"},
  { ch=1, img="char6_new/normal" , text="…？\n(気づいたのかな？)"},
  { ch=2, img="char4_new/normal" , text="なあに？\nアナタもワタクシのファンでして？"},
  { ch=1, img="char6_new/normal" , text="…いえ。\n(どうやらただのバカだね。)"},
  { ch=2, img="char4_new/glad" , text="ワタクシの幻想的な絵は、\nアナタの魔術に負けませんわよ。"},
  { ch=1, img="char6_new/anger" , text="…ふん！\n(お手並みを拝見しようじゃない！)"},

  { ch=1, text="" }
}

local act6_act5_ = {
  { ch=2, img="char5_new/normal" , text="今日は外が騒がしいな。",board_flip="HV",effect_a="slide_in" },
  { ch=2, img="char5_new/glad" , text="だがこの位で心を\n乱してはいかんな！"},
  { ch=1, img="char6_new/normal" , text="…おい。\n(ちょっと探ってみよう)",board_flip="V",effect_a="slide_in" },
  { ch=2, img="char5_new/anger" , text="雑念を払って、\n集中だ！"},
  { ch=1, img="char6_new/glad" , text="…えい！\n(脅かしてやる！)"},
  { ch=2, img="char5_new/normal" , text="お、お客さんだな。"},
  { ch=2, img="char5_new/glad" , text="心身鍛錬に来たんだね？"},
  { ch=1, img="char6_new/normal" , text="…違う。\n(驚いてない？)"},
  { ch=2, img="char5_new/glad" , text="成功は一日にしてならず、\n実力の裏は、\n努力を続けることだ！"},
  { ch=2, img="char5_new/anger" , text="魔術みたいにはいかないのだよ！"},
  { ch=1, img="char6_new/anger" , text="ふん！\n(なら試してみよう！)"},

  { ch=1, text="" }
}

local act6_act6_ = {
  { ch=1, img="char6_new/normal" , text="(今日も魔術の練習をしましょう。)",board_flip="V",effect_a="slide_in" },
  { ch=1, img="char6_new/glad" , text="えい！"},
  { ch=1, img="char6_new/sad" , text="(ダメ！\nまた超能力使っちゃった…)",board_flip="V"},
  { ch=1, img="char6_new/normal" , text="(本当は手品でやるつもりなのに…)",board_flip="V"},
  { ch=1, img="char6_new/sad" , text="(いつもうっかりして、\n超能力を使っちゃう…)",board_flip="V"},
  { ch=1, img="char6_new/normal" , text="(もう一回やろう！)",board_flip="V"},
  { ch=1, img="char6_new/sad" , text="うッ！\n(痛い！) ",effect_w="shake", special={id="cube", x1=500, y1=-50, x2=270, y2=420, x3=-100, y3=-100, path='cubes/cube-r-1', w=64, h=64, dur=250},effect_w="size_M",board_flip="V"},
  { ch=1, img="char6_new/sad" , text="(何で\nまた超能力使っちゃったのよ…)"},
  { ch=1, img="char6_new/normal" , text="…え？\n(このキューブたちは？)",board_flip="V"},

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