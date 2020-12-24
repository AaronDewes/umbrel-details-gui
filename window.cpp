/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This logoFile is part of the examples of the Qt Wayland module
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this logoFile in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this logoFile under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "config.h"
#include "window.h"
#include "MainWidget.h"

#include "QrCode.hpp"

#include <QWidget>
#include <QWebEngineView>
#include <QVBoxLayout>
#include <QPushButton>

Window::Window()
	:QWidget()
{
}

void Window::handleButton() {
    layout()->removeItem(layout()->itemAt(0));
    layout()->removeItem(layout()->itemAt(0));
    layout()->removeItem(layout()->itemAt(0));
    QWebEngineView *view = new QWebEngineView(this);
    view->setUrl(QUrl("http://localhost"));
    view->show();
    QPushButton *QRButton = new QPushButton("Show QR code");
    QRButton->setMinimumHeight(75);
    connect(QRButton, &QPushButton::clicked, this, &Window::reinit);
    layout()->addWidget(view);
    layout()->addWidget(QRButton);
}

void Window::reinit() {
    layout()->removeItem(layout()->itemAt(0));
    layout()->removeItem(layout()->itemAt(0));
    layout()->removeItem(layout()->itemAt(0));
    MainWidget *mainWidget = new MainWidget();
    QPushButton *button = new QPushButton("Open the Umbrel dashboard");
    button->setMinimumHeight(75);
    layout()->addWidget(mainWidget);
    layout()->addWidget(button);
    connect(button, &QPushButton::clicked, this, &Window::handleButton);
}

void Window::init() {
    QVBoxLayout *layout = new QVBoxLayout;
    MainWidget *mainWidget = new MainWidget();
    layout->setSpacing(0);
    layout->setMargin(0);
    QPushButton *button = new QPushButton("Open the Umbrel dashboard");
    button->setMinimumHeight(75);
    layout->addWidget(mainWidget);
    layout->addWidget(button);
    setLayout(layout);
    connect(button, &QPushButton::clicked, this, &Window::handleButton);
}
