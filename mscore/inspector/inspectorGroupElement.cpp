//=============================================================================
//  MuseScore
//  Music Composition & Notation
//  $Id:$
//
//  Copyright (C) 2012 Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENSE.GPL
//=============================================================================

#include "libmscore/score.h"
#include "libmscore/element.h"
#include "inspector.h"
#include "inspectorGroupElement.h"

namespace Ms {

//---------------------------------------------------------
//   InspectorGroupElement
//---------------------------------------------------------

InspectorGroupElement::InspectorGroupElement(QWidget* parent)
   : InspectorBase(parent)
      {
      QWidget* w = new QWidget;
      ge.setupUi(w);
      _layout->insertWidget(_layout->count()-1, w);
      ge.color->setColor(Qt::black);
      connect(ge.setColor, SIGNAL(clicked()), SLOT(setColor()));
      connect(ge.setVisible, SIGNAL(clicked()), SLOT(setVisible()));
      connect(ge.setInvisible, SIGNAL(clicked()), SLOT(setInvisible()));

      //
      // Select
      //
      QLabel* l = new QLabel;
      l->setText(tr("Select"));
      QFont font(l->font());
      font.setBold(true);
      l->setFont(font);
      l->setAlignment(Qt::AlignHCenter);
      _layout->addWidget(l);
      QFrame* f = new QFrame;
      f->setFrameStyle(QFrame::HLine | QFrame::Raised);
      f->setLineWidth(2);
      _layout->addWidget(f);
      QHBoxLayout* hbox = new QHBoxLayout;

      notes = new QToolButton(this);
      notes->setText(tr("Notes"));
      notes->setEnabled(true);
      hbox->addWidget(notes);

      rests = new QToolButton(this);
      rests->setText(tr("Rests"));
      rests->setEnabled(true);
      hbox->addWidget(rests);

      _layout->addLayout(hbox);
      connect(notes, SIGNAL(clicked()), SLOT(notesClicked()));
      connect(rests, SIGNAL(clicked()), SLOT(restsClicked()));
      }

//---------------------------------------------------------
//   setColor
//---------------------------------------------------------

void InspectorGroupElement::setColor()
      {
      if (inspector->el().isEmpty())
            return;
      Score* score = inspector->el().front()->score();
      score->startCmd();
      for (Element* e : inspector->el()) {
            if (e->getProperty(P_ID::COLOR) != QVariant(ge.color->color()))
                  e->undoChangeProperty(P_ID::COLOR, ge.color->color());
            }
      score->endCmd();
      }

//---------------------------------------------------------
//   setVisible
//---------------------------------------------------------

void InspectorGroupElement::setVisible()
      {
      if (inspector->el().isEmpty())
            return;
      Score* score = inspector->el().front()->score();
      score->startCmd();
      for (Element* e : inspector->el()) {
            if (!e->getProperty(P_ID::VISIBLE).toBool())
                  e->undoChangeProperty(P_ID::VISIBLE, true);
            }
      score->endCmd();
      }

//---------------------------------------------------------
//   setInvisible
//---------------------------------------------------------

void InspectorGroupElement::setInvisible()
      {
      if (inspector->el().isEmpty())
            return;
      Score* score = inspector->el().front()->score();
      score->startCmd();
      for (Element* e : inspector->el()) {
            if (e->getProperty(P_ID::VISIBLE).toBool())
                  e->undoChangeProperty(P_ID::VISIBLE, false);
            }
      score->endCmd();
      }

//---------------------------------------------------------
//   notesClicked
//---------------------------------------------------------

void InspectorGroupElement::notesClicked()
      {
      Score* score = inspector->el().front()->score();
      QList<Element*> el = score->selection().elements();
      QList<Element*> nel;
      score->deselectAll();
      for (Element* e : el) {
            if (e->type() == Element::Type::NOTE) {
                  nel.append(e);
                  score->selection().add(e);
                  }
            }
      inspector->setElements(nel);
      score->update();
      }

//---------------------------------------------------------
//   restsClicked
//---------------------------------------------------------

void InspectorGroupElement::restsClicked()
      {
      Score* score = inspector->el().front()->score();
      QList<Element*> el = score->selection().elements();
      QList<Element*> nel;
      score->deselectAll();
      for (Element* e : el) {
            if (e->type() == Element::Type::REST) {
                  nel.append(e);
                  score->selection().add(e);
                  }
            }
      inspector->setElements(nel);
      score->update();
      }

}

