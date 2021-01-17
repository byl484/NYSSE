#include <QtTest>
#include "../Game/statistics.hh"


class StatisticsTest : public QObject
{
    Q_OBJECT

public:
    StatisticsTest();
    ~StatisticsTest();

private slots:
    void testSetDifficulty();
    void testRemoveLife();
    void testPlayerIsDead();

};


StatisticsTest::StatisticsTest()
{

}


StatisticsTest::~StatisticsTest()
{

}


void StatisticsTest::testSetDifficulty()
{
    Statistics s;

    s.setDifficulty(0);
    QCOMPARE(s.giveDifficulty().difficulty_name, EASY.difficulty_name);
    s.setDifficulty(1);
    QCOMPARE(s.giveDifficulty().difficulty_name, MEDIUM.difficulty_name);
    s.setDifficulty(2);
    QCOMPARE(s.giveDifficulty().difficulty_name, HARD.difficulty_name);
    s.setDifficulty(3);
    QCOMPARE(s.giveDifficulty().difficulty_name, INSANE.difficulty_name);
}


void StatisticsTest::testRemoveLife()
{
    Statistics s;
    QCOMPARE(s.givePlayerLives(), 3);
    s.removeLife();
    QCOMPARE(s.givePlayerLives(), 2);
    s.removeLife();
    QCOMPARE(s.givePlayerLives(), 1);
    s.removeLife();
    QCOMPARE(s.givePlayerLives(), 0);
    s.removeLife();
    QCOMPARE(s.givePlayerLives(), 0); // Lives can't go negative.
}


void StatisticsTest::testPlayerIsDead()
{
    Statistics s;
    QCOMPARE(s.playerIsDead(), false);
    s.removeLife();
    s.removeLife();
    s.removeLife();
    QCOMPARE(s.playerIsDead(), true);
}


QTEST_APPLESS_MAIN(StatisticsTest)

#include "tst_statisticstest.moc"
