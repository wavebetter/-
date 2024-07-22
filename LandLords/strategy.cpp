#include "strategy.h"
#include <QMap>
#include<QDebug>
Cards Strategy::makeStrategy()
{
   //得到出牌玩家对象和它打出的牌
   Player* pendPlayer=m_player->getPendPlayer();
   Cards pendCards=m_player->getPendCards();

   //判断上次出牌的玩家是不是自己
   if(pendPlayer==m_player||pendPlayer==nullptr){
       //直接出牌
       return firstPlay();
   }else{
       //不是自己出的要找比出牌玩家点数更大的牌
       PlayHand type(pendCards);
       Cards beatsCards=getGreaterCards(type);
       //找到了更大的判断是否出牌
       bool shouldBeat=whetherToBeat(beatsCards);
       if(shouldBeat){
           return beatsCards;
       }else{
           return Cards();
       }
   }
   return Cards();
}

Cards Strategy::firstPlay()
{
    // 判断玩家手中是否只剩单一的牌型
    PlayHand hand(m_cards);
    if(hand.getHandType() != PlayHand::Hand_Unknown)
    {
        return m_cards;
    }
    // 不是单一牌型
    // 判断玩家手中是否有顺子
    QVector<Cards> optimalSeq = pickOptimalSeqSingles();
    if(!optimalSeq.isEmpty())
    {
        // 得到单牌的数量
        int baseNum = findCardsByCount(1).size();
        // 把得到的顺子的集合从玩家手中删除
        Cards save = m_cards;
        save.remove(optimalSeq);
        int lastNum = Strategy(m_player, save).findCardsByCount(1).size();
        if(baseNum > lastNum)
        {
            qDebug()<<m_player<<"准备打出顺子";
            return optimalSeq[0];
        }
    }

    bool hasPlane, hasTriple, hasPair;
    hasPair = hasTriple = hasPlane = false;
    Cards backup = m_cards;

    // 有没有炸弹
    QVector<Cards> bombArray = findCardType(PlayHand(PlayHand::Hand_Bomb, Card::Card_Begin, 0), false);
    backup.remove(bombArray);

    // 有没有飞机
    QVector<Cards> planeArray = Strategy(m_player, backup).findCardType(PlayHand(PlayHand::Hand_Plane, Card::Card_Begin, 0), false);
    if(!planeArray.isEmpty())
    {
        hasPlane = true;
        backup.remove(planeArray);
    }

    // 有没有三张点数相同的牌
    QVector<Cards> seqTripleArray = Strategy(m_player, backup).findCardType(PlayHand(PlayHand::Hand_Triple, Card::Card_Begin, 0), false);
    if(!seqTripleArray.isEmpty())
    {
        qDebug()<<" !seqTripleArray.isEmpty()";
        hasTriple = true;
        backup.remove(seqTripleArray);
    }

    // 有没有连对
    QVector<Cards> seqPairArray = Strategy(m_player, backup).
                findCardType(PlayHand(PlayHand::Hand_Seq_Pair, Card::Card_Begin, 0), false);
    if(!seqPairArray.isEmpty())
    {
        hasPair = true;
        backup.remove(seqPairArray);
    }

    if(hasPair)
    {
        Cards maxPair;
        for(int i=0; i<seqPairArray.size(); ++i)
        {
            if(seqPairArray[i].CardCount() > maxPair.CardCount())
            {
                maxPair = seqPairArray[i];
            }
        }
        qDebug()<<m_player<<"准备打出对子";
        return maxPair;
    }

    if(hasPlane)
    {
        // 1. 飞机带两个对儿
        bool twoPairFond = false;
        QVector<Cards> pairArray;
        for(Card::CardPoint point = Card::Card_3; point <= Card::Card_10; point = Card::CardPoint(point + 1))
        {
            Cards pair = Strategy(m_player, backup).findSamePointcards(point, 2);
            if(!pair.isEmpty())
            {
                pairArray.push_back(pair);
                if(pairArray.size() == 2)
                {
                    twoPairFond = true;
                    break;
                }
            }
        }
        if(twoPairFond)
        {
            Cards tmp = planeArray[0];
            tmp.add(pairArray);
            qDebug()<<m_player<<"准备打出飞机带两个对";
            return tmp;
        }
        // 2. 飞机带两个单牌
        else
        {
            bool twoSingleFond = false;
            QVector<Cards> singleArray;
            for(Card::CardPoint point = Card::Card_3; point <= Card::Card_10; point = Card::CardPoint(point + 1))
            {
                if(backup.pointCount(point) == 1)
                {
                    Cards single = Strategy(m_player, backup).findSamePointcards(point, 1);
                    if(!single.isEmpty())
                    {
                        singleArray.push_back(single);
                        if(singleArray.size() == 2)
                        {
                            twoSingleFond = true;
                            break;
                        }
                    }
                }
            }
            if(twoSingleFond)
            {
                Cards tmp = planeArray[0];
                tmp.add(singleArray);
                qDebug()<<m_player<<"准备飞机带两个单";
                return tmp;
            }
            else
            {
                // 3. 飞机
                qDebug()<<m_player<<"准备打出飞机";
                return planeArray[0];
            }
        }
    }

    if(hasTriple)

    {
        if(PlayHand(seqTripleArray[0]).getCardPoint() < Card::Card_A)
        {
            for(Card::CardPoint point = Card::Card_3; point <= Card::Card_A; point = Card::CardPoint(point+1))
            {
                int pointCount = backup.pointCount(point);
                if(pointCount == 1)
                {
                    Cards single = Strategy(m_player, backup).findSamePointcards(point, 1);
                    Cards tmp = seqTripleArray[0];
                    tmp.add(single);
                    qDebug()<<m_player<<"准备打出三带一";
                    return tmp;
                }
                else if(pointCount == 2)
                {
                    Cards pair = Strategy(m_player, backup).findSamePointcards(point, 2);
                    Cards tmp = seqTripleArray[0];
                    tmp.add(pair);
                    qDebug()<<m_player<<"准备三带对";
                    return tmp;
                }
            }
        }
        // 不带副牌
        qDebug()<<"准备打出三张相同的";
        return seqTripleArray[0];
    }
    // 单牌或者对牌
    Player* nextPlayer = m_player->getNextPlayer();
    if(nextPlayer->getCards().CardCount() == 1 && m_player->getRole() != nextPlayer->getRole())
    {
        qDebug()<<"nextPlayer->getCards().CardCount() == 1 && m_player->getRole() != nextPlayer->getRole()";
        for(Card::CardPoint point = Card::CardPoint(Card::Card_End-1);
             point >= Card::Card_3; point = Card::CardPoint(point-1))
        {
            int pointCount = backup.pointCount(point);
            if(pointCount == 1)
            {
                Cards single = Strategy(m_player, backup).findSamePointcards(point, 1);
                qDebug()<<m_player<<"准备打出单牌";
                return single;
            }
            else if(pointCount == 2)
            {
                Cards pair = Strategy(m_player, backup).findSamePointcards(point, 2);
                qDebug()<<m_player<<"准备打出对子";
                return pair;
            }
        }
    }
    else
    {
        qDebug()<<"else";
        for(Card::CardPoint point =  Card::Card_3;
             point < Card::Card_End; point = Card::CardPoint(point+1))
        {
            int pointCount = backup.pointCount(point);
            if(pointCount == 1)
            {
                Cards single = Strategy(m_player, backup).findSamePointcards(point, 1);
                qDebug()<<m_player<<"准备打出单牌";
                return single;
            }
            else if(pointCount == 2)
            {
                Cards pair = Strategy(m_player, backup).findSamePointcards(point, 2);
                qDebug()<<m_player<<"准备打出对子";
                return pair;
            }
        }
    }
    return Cards();
}
Cards Strategy::getGreaterCards(PlayHand type)
{
    //1.出牌玩家和当前玩家是不是队友
    Player* pendPlayer=m_player->getPendPlayer();
    if(pendPlayer != nullptr && pendPlayer->getRole() != m_player->getRole() && pendPlayer->getCards().CardCount() <= 3){
        QVector<Cards> bombs=findCardsByCount(4);
        for(int i=0;i<bombs.size();++i){
            if(PlayHand(bombs[i]).canBeat(type)){
                return bombs[i];
            }
        }
        //看看有没有王炸
        Cards sj=findSamePointcards(Card::Card_SJ,1);
        Cards bj=findSamePointcards(Card::Card_BJ,1);
        if(!sj.isEmpty()&&!bj.isEmpty()){
            Cards jokers;
            jokers<<sj<<bj;
            return jokers;
        }
    }
    //2.当前玩家和下一个玩家是不是队友
    Player* nextPlayer=m_player->getNextPlayer();
    //将顺子剔除出去
    Cards remain=m_cards;
    remain.remove(Strategy(m_player,remain).pickOptimalSeqSingles());
    QVector<Cards> beatCardsArray=Strategy(m_player,remain).findCardType(type,true);
    if(!beatCardsArray.isEmpty()){
        if(m_player->getRole()!=nextPlayer->getRole()&&nextPlayer->getCards().CardCount()<=2){
            return beatCardsArray.back();//直接拿最大的干它
        }else{
            return beatCardsArray.front();
        }
    }else{
        QVector<Cards> beatCardsArray=Strategy(m_player,m_cards).findCardType(type,true);
        if(!beatCardsArray.isEmpty()){
            if(m_player->getRole()!=nextPlayer->getRole()&&nextPlayer->getCards().CardCount()<=2){
                return beatCardsArray.back();
            }else{
                return beatCardsArray.front();
            }
    }
        return Cards();
    }
}

bool Strategy::whetherToBeat(Cards &cs)
{
    //没有更大的牌
    if(cs.isEmpty()){
        return false;
    }
    //得到出牌玩家的对象
    Player*pendPlayer =m_player->getPendPlayer();
    if(m_player->getRole()==pendPlayer->getRole()){//这是队友出的牌
        //手里的牌所剩无几且完整
        Cards left=m_cards;
        left.remove(cs);
        if(PlayHand(left).getHandType()!=PlayHand::Hand_Unknown){
            return true;
        }
        //手里的牌都很大
        Card::CardPoint basePoint =PlayHand(cs).getCardPoint();
        if(basePoint==Card::Card_2||basePoint==Card::Card_SJ||basePoint==Card::Card_BJ){
            return false;
        }
    }else{
        //保存实力的情况
        PlayHand myhand(cs);
        if(myhand.getHandType()==PlayHand::Hand_Triple_Single||myhand.getHandType()==PlayHand::Hand_Triple_Pair
            &&myhand.getCardPoint()==Card::Card_2){
            return false;
        }
        //如果CS是对2，并且出牌玩家手里的牌数量大于10&&自己手里的牌的数量大于5.放弃出牌
        if(myhand.getHandType()==PlayHand::Hand_Pair&&myhand.getCardPoint()==Card::Card_2
            &&pendPlayer->getCards().CardCount()>=10&&m_player->getCards().CardCount()>=5){
            return false;
        }
    }
}

Cards Strategy::findSamePointcards(Card::CardPoint point, int count)
{
    if(count<1||count>4){
        return Cards();
    }
    //大小王
    if(point==Card::Card_SJ||point==Card::Card_BJ){
        if(count>1){
            return Cards();
        }
        Card card;
        card.setPoint(point);
        card.setSuit(Card::Suit_Begin);
        if(m_cards.contains(card)){
            Cards cards;
            cards.add(card);
            return cards;
        }
    }
    //不是大小王
    int findCount =0;
    Cards findcards;
    for(int suit=Card::Suit_Begin+1;suit<Card::Suit_End;++suit){
        Card card;
        card.setPoint(point);
        card.setSuit((Card::CardSuit)suit);
        if(m_cards.contains(card)){
            findCount++;
            findcards.add(card);
            if(findCount==count){
                return findcards;
            }
        }
    }
    return Cards();
}

QVector<Cards> Strategy::findCardsByCount(int count)
{
    if(count<1||count>4){
        return QVector<Cards>();
    }
    QVector<Cards> cardsArray;
    for(Card::CardPoint point=Card::Card_3;point<Card::Card_End;point=(Card::CardPoint)(point+1)){
        if(m_cards.pointCount(point)==count){
            Cards cs;
            cs<<findSamePointcards(point,count);
            cardsArray<<cs;
        }
    }
    return cardsArray;
}

Cards Strategy::getRangeCards(Card::CardPoint begin, Card::CardPoint end)
{
    Cards rangeCards;
    for(Card::CardPoint point=begin;point<end;point=(Card::CardPoint)(point+1)){
        int count=m_cards.pointCount(point);
        Cards cs=findSamePointcards(point,count);
        rangeCards<<cs;
    }
    return rangeCards;
}

QVector<Cards> Strategy::findCardType(PlayHand hand, bool beat)
{
    PlayHand::HandType type=hand.getHandType();
    Card::CardPoint point=hand.getCardPoint();
    int extra=hand.getExtra();

    //确定起始点数
    Card::CardPoint beginPoint=beat?Card::CardPoint(point+1):Card::Card_3;
    switch(type){
    case PlayHand::Hand_Single:
        return getCards(beginPoint, 1);
    case PlayHand::Hand_Pair:
        return getCards(beginPoint, 2);
    case PlayHand::Hand_Triple:
        return getCards(beginPoint, 3);
    case PlayHand::Hand_Triple_Single:
        return getTripleSingleOrPair(beginPoint, PlayHand::Hand_Single);
    case PlayHand::Hand_Triple_Pair:
        return getTripleSingleOrPair(beginPoint, PlayHand::Hand_Pair);
    case PlayHand::Hand_Plane:
        return getPlane(beginPoint);
    case PlayHand::Hand_Plane_Two_Single:
        return getPlane2SingleOr2Pair(beginPoint, PlayHand::Hand_Single);
    case PlayHand::Hand_Plane_Two_Pair:
        return getPlane2SingleOr2Pair(beginPoint, PlayHand::Hand_Pair);
    case PlayHand::Hand_Seq_Pair:
    {
        CardInfo info;
        info.begin = beginPoint;
        info.end = Card::Card_Q;
        info.number = 2;
        info.base = 3;
        info.extra = extra;
        info.beat = beat;
        info.getSeq = &Strategy::getBaseSeqPair;
        return getSepPairOrSeqSingle(info);
    }
    case PlayHand::Hand_Seq_Single:
    {
        CardInfo info;
        info.begin = beginPoint;
        info.end = Card::Card_10;
        info.number = 1;
        info.base = 5;
        info.extra = extra;
        info.beat = beat;
        info.getSeq = &Strategy::getBaseSeqSingle;
        return getSepPairOrSeqSingle(info);
    }
    case PlayHand::Hand_Bomb:
        return getBomb(beginPoint);
    default:
        return QVector<Cards>();

    }
}

void Strategy::pickSeqSingles(QVector<QVector<Cards>> &allSeqRecord, const QVector<Cards> &seqSingle, const Cards &cards)
{
    // 1. 得到所有顺子的组合
    QVector<Cards> allSeq = Strategy(m_player, cards).findCardType(PlayHand(PlayHand::Hand_Seq_Single, Card::Card_Begin, 0), false);
    if(allSeq.isEmpty())
    {
        // 结束递归,将满足条件的顺子传递给调用者
        allSeqRecord << seqSingle;
    }
    else    // 2. 对顺子进行筛选
    {
        Cards saveCards = cards;
        // 遍历得到的所有的顺子
        for(int i=0; i<allSeq.size(); ++i)
        {
            // 将顺子取出
            Cards aScheme = allSeq.at(i);
            // 将顺子从用户手中删除
            Cards temp = saveCards;
            temp.remove(aScheme);

            QVector<Cards> seqArray = seqSingle;
            seqArray << aScheme;

            // 检测还有没有其他的顺子
            // seqArray 存储一轮for循环中多轮递归得到的所有的可用的顺子
            // allSeqRecord 存储多轮for循环中多轮递归得到的所有的可用的顺子
            pickSeqSingles(allSeqRecord, seqArray, temp);
        }
    }

}

QVector<Cards> Strategy::pickOptimalSeqSingles()
{
    QVector<QVector<Cards>> seqRecord;
    QVector<Cards> seqSingles;
    Cards save = m_cards;
    save.remove(findCardsByCount(4));
    save.remove(findCardsByCount(3));
    pickSeqSingles(seqRecord, seqSingles, save);
    if(seqRecord.isEmpty())
    {
        return QVector<Cards>();
    }

    // 遍历容器
    QMap<int, int> seqMarks;
    for(int i=0; i<seqRecord.size(); ++i)
    {
        Cards backupCards = m_cards;
        QVector<Cards> seqArray = seqRecord[i];
        backupCards.remove(seqArray);

        // 判断剩下的单牌数量, 数量越少,顺子的组合就越合理
        QVector<Cards> singleArray = Strategy(m_player, backupCards).findCardsByCount(1);

        CardList cardList;
        for(int j=0; j<singleArray.size(); ++j)
        {
            cardList << singleArray[j].toCardList();
        }
        // 找点数相对较大一点顺子
        int mark = 0;
        for(int j=0; j<cardList.size(); ++j)
        {
            mark += cardList[j].point() + 15;
        }
        seqMarks.insert(i, mark);
    }

    // 遍历map
    int value = 0;
    int comMark = 1000;
    auto it = seqMarks.constBegin();
    for(;it!=seqMarks.constEnd(); ++it)
    {
        if(it.value() < comMark)
        {
            comMark = it.value();
            value = it.key();
        }
    }

    return seqRecord[value];
}

QVector<Cards> Strategy::getCards(Card::CardPoint point, int number)
{
    QVector<Cards> findCardsArray;
    for(Card::CardPoint pt=point;pt<Card::Card_End;pt=(Card::CardPoint)(pt+1)){
        //尽量不拆分牌型
        if(m_cards.pointCount(pt)==number){
            Cards cs=findSamePointcards(pt,number);
            if(!cs.isEmpty()){
                findCardsArray<<cs;
            }
        }
    }
    return findCardsArray;
}

QVector<Cards> Strategy::getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type)
{
    //找到三张点数相同的牌
    QVector<Cards> findCardArray =getCards(begin,3);
    if(!findCardArray.isEmpty()){
        //将找到的牌从用户手中删除
        Cards remainCards=m_cards;
        remainCards.remove(findCardArray);
        //搜索牌型
        Strategy st(m_player,remainCards);
        QVector<Cards> cardsArray=st.findCardType(PlayHand(type,Card::Card_Begin,0),false);
        if(!cardsArray.isEmpty()){
            //将找到的牌和三张点数相同的牌进行组合
            for(int i=0;i<findCardArray.size();++i){
                findCardArray[i].add(cardsArray.at(i));
            }
        }else{
            findCardArray.clear();
        }
    }
    //将最终结果返回
    return findCardArray;
}

QVector<Cards> Strategy::getPlane(Card::CardPoint begin)
{
    QVector<Cards> findCardArray;
    for(Card::CardPoint point=begin;point<=Card::Card_K;point=(Card::CardPoint)(point+1)){
        //根据点数和数量进行搜索
        Cards prevCards=findSamePointcards(point,3);
        Cards nextCards=findSamePointcards((Card::CardPoint)(point+1),3);
        if(!prevCards.isEmpty()&&!nextCards.isEmpty()){
            Cards tmp;
            tmp<<prevCards<<nextCards;
            findCardArray<<tmp;
        }
    }
    return findCardArray;
}

QVector<Cards> Strategy::getPlane2SingleOr2Pair(Card::CardPoint begin, PlayHand::HandType type)
{
    //找飞机
    QVector<Cards> findCardArray=getPlane(begin);
    if(!findCardArray.isEmpty()){
        //将找到的牌从用户手中删除
        Cards remainCards=m_cards;
        remainCards.remove(findCardArray);
        //搜索牌型
        Strategy st(m_player,remainCards);
        QVector<Cards> cardsArray=st.findCardType(PlayHand(type,Card::Card_Begin,0),false);
        if(!cardsArray.size()>=2){
            //将找到的牌和三张点数相同的牌进行组合
            for(int i=0;i<findCardArray.size();++i){
                Cards tmp;
                tmp<<cardsArray[0]<<cardsArray[1];
                findCardArray[i].add(tmp);
            }
        }else{
            findCardArray.clear();
        }
    }
    return findCardArray;
}

QVector<Cards> Strategy::getSepPairOrSeqSingle(CardInfo&info)
{
    QVector<Cards> findCardsArray;
    if(info.beat){
        for(Card::CardPoint point=info.begin;point<=info.end;point=(Card::CardPoint)(point+1)){
            bool found=true;
            Cards seqCards;
            for(int i=0;i<info.extra;++i){
                //按点数和数量搜索
                Cards cards=findSamePointcards((Card::CardPoint)(point+i),info.number);
                if(cards.isEmpty()||(point+info.extra>=Card::Card_2)){
                    found=false;
                    seqCards.clear();
                    break;
                }else{
                    seqCards<<cards;
                }
            }
            if(found){
                findCardsArray<<seqCards;
                return findCardsArray;
            }
        }
        return findCardsArray;
    }
    for(Card::CardPoint point=info.begin;point<=info.end;point=(Card::CardPoint)(point+1)){

        //把找到的基础连对存一下
        Cards baseSep=(this->*info.getSeq)(point);
        if(baseSep.isEmpty()){
            continue;
        }
        //连对存储到容器中
        findCardsArray<<baseSep;
        int followed=info.base;
        Cards alreadyFollowCrads;//存储后续找到的满足条件的连对
        while(true){
            //新的起始点数
            Card::CardPoint followedPoint=Card::CardPoint(point+followed);
            //判断是否超出上限
            if(followedPoint>=Card::Card_2){
                break;
            }
            Cards followedCards=findSamePointcards(followedPoint,info.number);
            if(followedCards.isEmpty()){
                break;
            }else{
                alreadyFollowCrads<<followedCards;
                Cards newSeq=baseSep;
                newSeq<<alreadyFollowCrads;
                findCardsArray<<newSeq;
                followed++;
            }
        }
    }
    return findCardsArray;
}

QVector<Cards> Strategy::getBomb(Card::CardPoint begin)
{
    QVector<Cards> findcardsArray;
    for(Card::CardPoint point=begin;point<Card::Card_End;point=(Card::CardPoint)(point+1)){
        Cards cs=findSamePointcards(point,4);
        if(!cs.isEmpty()){
            findcardsArray<<cs;
        }
    }
    return findcardsArray;
}

Cards Strategy::getBaseSeqPair(Card::CardPoint point)
{
    //找到三个点数连续的对
    Cards cards0=findSamePointcards(point,2);
    Cards cards1=findSamePointcards((Card::CardPoint)(point+1),2);
    Cards cards2=findSamePointcards((Card::CardPoint)(point+2),2);
    Cards baseSeq;
    if(!cards0.isEmpty()&&!cards1.isEmpty()&&!cards2.isEmpty()){
        baseSeq<<cards0<<cards1<<cards2;
        }
    return baseSeq;
}


Cards Strategy::getBaseSeqSingle(Card::CardPoint point)
{
    //找到5个点数连续的单
    Cards cards0=findSamePointcards(point,1);
    Cards cards1=findSamePointcards((Card::CardPoint)(point+1),1);
    Cards cards2=findSamePointcards((Card::CardPoint)(point+2),1);
    Cards cards3=findSamePointcards((Card::CardPoint)(point+1),1);
    Cards cards4=findSamePointcards((Card::CardPoint)(point+1),1);
    Cards baseSeq;
    if(!cards0.isEmpty()&&!cards1.isEmpty()&&!cards2.isEmpty()&&!cards3.isEmpty()&&!cards4.isEmpty()){
        baseSeq<<cards0<<cards1<<cards2<<cards3<<cards4;
    }
    return baseSeq;
}
