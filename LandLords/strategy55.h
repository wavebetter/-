#ifndef STRATEGY_H
#define STRATEGY_H
#include "player.h"
#include "playhand.h"
class Strategy
{
private:
    using function=Cards(Strategy::*)(Card::CardPoint point);
    struct CardInfo{
        Card::CardPoint begin;
        Card::CardPoint end;
        int extra;//顺子或者连对的数量（几连）
        bool beat;
        int number;//指定点数牌的数量
        int base;//基本的顺子或者连对的数量
        function getSeq;
    };
public:
    Strategy(Player* player,const Cards& cards):m_player(player),m_cards(cards){};
    //1.指定出牌策略
    Cards makeStrategy();
    //2.第一个出牌
    Cards firstPlay();
    //3.得到比指定牌大的牌型
    Cards getGreaterCards(PlayHand type);
    //4.能管上的时候，判断管不管
    bool whetherToBeat(Cards&cs);
    //5.找出指定数量的相同点数的牌
    Cards findSamePointcards(Card::CardPoint point,int count);
    //6.找出所有点数数量为count的牌
    QVector<Cards> findCardsByCount(int count);
    //7.根据点数范围找牌
    Cards getRangeCards(Card::CardPoint begin,Card::CardPoint end);
    //8.按照牌型找牌，并且指定要找的牌是否大过指定的牌型
    QVector<Cards> findCardType(PlayHand hand, bool beat);
    //9.从指定的Cards对象中把顺子挑选出来
    void pickSeqSingles(QVector<QVector<Cards>> &allSeqRecord, const QVector<Cards> &seqSingle, const Cards &cards);
    //10.最优顺子的集合的筛选函数
    QVector<Cards> pickOptimalSeqSingles();
private:

    QVector<Cards> getCards(Card::CardPoint point,int number);
    QVector<Cards> getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type);
    QVector<Cards> getPlane(Card::CardPoint begin);
    QVector<Cards> getPlane2SingleOr2Pair(Card::CardPoint begin, PlayHand::HandType type);
    QVector<Cards> getSepPairOrSeqSingle(CardInfo& info);
    QVector<Cards> getBomb(Card::CardPoint begin);
    Cards getBaseSeqPair(Card::CardPoint point);
    Cards getBaseSeqSingle(Card::CardPoint point);

private:
    Player *m_player;
    Cards m_cards;
};

#endif // STRATEGY_H
