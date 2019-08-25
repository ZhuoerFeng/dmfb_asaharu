#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QVector>
#include <QPoint>
#include <QObject>


class Chip;

enum CommandType {INPUT, MOVE, SPLIT_0, SPLIT_1, MERGE_0, MERGE_1, OUTPUT};
enum CommandDirection {UP, DOWN, LEFT, RIGHT, NO};

class Command
{
public:
    Command();
//    Command(QPoint, QString&, int, CommandType);
    Command createCommand(QString type, int timestamp, int curx, int cury, CommandDirection direction);

    void setTime(int);
    void setType(CommandType);
    void setPosition(int, int);
    void setDirection(CommandDirection);

    int doCommand(Chip* cur_chip);

    CommandType getType();
    int getTime();
    QPoint getPosition();
    CommandDirection getDirection();

    void printCommand();

    friend bool operator< (const Command&, const Command&);
//    static QVector<Command> parse_command_set(QString&);

//    QString name;
    QPoint position;
    CommandDirection direction;
    CommandType type;
    int timestamp;
};


class CommandFactory : public QObject
{
    Q_OBJECT

public:
    void init(QString commandText, Chip *initialBoard);
    bool reachEnd();
    bool reachHead();

    void next();
    void prev();

    void reset();

    bool check_status();
    Chip* get_chip();
    bool inWashMode();
    QString filen;

signals:
    void chip_changed();
    void error_found();


public:
    QVector<Command> command;

    QVector<Chip*> AllChips;
    bool washMode = false;
    int maxt = -1;
    int mint = INT_MAX;
    int curt = 0;
    QVector<Command>::iterator commandIter;
};


#endif // COMMAND_H
