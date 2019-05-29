#ifndef TOTOLABEL_H
#define TOTOLABEL_H
#include <QLabel>
#include <QMouseEvent>

class TotoLabel : public QLabel
{
    Q_OBJECT

public:
    TotoLabel( QWidget* parent = 0, Qt::WindowFlags f = 0 ) : QLabel( parent, f ) {}
    TotoLabel( const QString& text, QWidget* parent = 0, Qt::WindowFlags f = 0 ): QLabel( text, parent, f ) {}
signals:
    void moussePressed(const QPoint );

private:
    void mousePressEvent( QMouseEvent* ev )
        {
            const QPoint p = ev->pos();
            emit moussePressed( p );
        }
};

#endif // TOTOLABEL_H
