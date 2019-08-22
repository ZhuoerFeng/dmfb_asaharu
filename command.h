#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QVector>
#include <QPoint>

class Chip;

enum CommandType {MOVE, SPLIT_0, SPLIT_1, MERGE_0, MERGE_1, INPUT, OUTPUT};
enum CommandDirection {UP, DOWN, LEFT, RIGHT, NO};

class Command
{
public:
    Command();
//    Command(QPoint, QString&, int, CommandType);
    ~Command();
//    Command(Command&) = delete;

    void setType(CommandType);
    void setPosition(int, int);
    void setTime(int);
    void setDirection(CommandDirection);

//    void doCommand();

    CommandType getType();
    QPoint getPosition();
    int getTime();
    CommandDirection getDirection();

    void printCommand();

    friend bool operator< (const Command&, const Command&);

//    QString name;
    QPoint position;
    CommandDirection direction;
    CommandType type;
    int timestamp;

    static QVector<Command> parse_command_set(QString&);
};



#endif // COMMAND_H
