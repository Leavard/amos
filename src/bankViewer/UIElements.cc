#include "UIElements.hh"

QColor UIElements::color_a(Qt::darkGreen);
QColor UIElements::color_c(Qt::blue);
QColor UIElements::color_g(Qt::yellow);
QColor UIElements::color_t(Qt::red);
QColor UIElements::color_default(Qt::black);
QColor UIElements::color_discrepancy(153, 102, 255);

QColor UIElements::color_Happy(Qt::green);
QColor UIElements::color_SizeViolation(Qt::blue);
QColor UIElements::color_LinkingMate(Qt::yellow);
QColor UIElements::color_OrientationViolation(Qt::red);
QColor UIElements::color_MissingMate(Qt::magenta);
QColor UIElements::color_NoMate(Qt::cyan);
QColor UIElements::color_Unknown(Qt::white);

QColor & UIElements::getBaseColor(char base)
{
  switch (base)
  {
    case 'A':
    case 'a': return color_a;

    case 'C':
    case 'c': return color_c;
    
    case 'G':
    case 'g': return color_g;

    case 'T':
    case 't': return color_t;

    default: return color_default;
  };
}

void UIElements::setBasePen(QPen & pen, char base)
{
  pen.setColor(getBaseColor(base));
}


QColor & UIElements::getInsertColor(Insert::MateState state)
{
  switch (state)
  {
    case Insert::Happy:                return color_Happy;
    case Insert::SizeViolation:        return color_SizeViolation;
    case Insert::LinkingMate:          return color_LinkingMate;
    case Insert::OrientationViolation: return color_OrientationViolation;
    case Insert::MissingMate:          return color_MissingMate;
    case Insert::NoMate:               return color_NoMate;

    default: return color_Unknown;
  };
}
