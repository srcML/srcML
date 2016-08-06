#include <QObject>

class Counter : public QObject
{
    Q_OBJECT

public:
    Counter() { m_value = 0; }
    int value() const { return m_value; }

public slots:
    void setValue(int value) { emit valueChanged(value); }

signals:
    void valueChanged(int newValue);

private:
    int m_value;
};

int main() {
  Counter a, b;
    QObject::connect(&a, SIGNAL(valueChanged(int)), &b, SLOT(setValue(int)));
    a.setValue(12);
    b.setValue(48);
}
