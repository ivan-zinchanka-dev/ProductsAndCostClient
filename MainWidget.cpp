#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    _networkManager = shared_ptr<NetworkManager>(new NetworkManager(1280, "127.0.0.1"));

    if (!_networkManager->ConnectToServer()){

        QMessageBox messageBox;
        messageBox.critical(this, "Error", CONNECTION_ERROR_MSG, QMessageBox::Ok);
        messageBox.setFixedSize(500, 200);

        throw exception(CONNECTION_ERROR_MSG);
    }

    _mainLayout = new QVBoxLayout(this);

    _customer = shared_ptr<Customer>(new Customer(this, _mainLayout));

    _buttonsLayout = new QHBoxLayout(this);

    InitButton(_addItemButton, "Add item", &MainWidget::ShowAddItemForm);
    InitButton(_editItemButton, "Edit item", &MainWidget::ShowEditItemForm);
    InitButton(_removeItemButton, "Remove item", &MainWidget::RemoveItemFromQuery);
    InitButton(_confirmItemButton, "Confirm query", nullptr);
    InitButton(_clearQueryButton, "Clear query", &MainWidget::ClearQuery);



    _mainLayout->addLayout(_buttonsLayout);

    setLayout(_mainLayout);

}

void MainWidget::InitButton(QPushButton* instance, QString text, void(MainWidget::*onClickFunc)())
{
    instance = new QPushButton(text);
    _buttonsLayout->addWidget(instance);
    QObject::connect(instance, &QPushButton::clicked, this, onClickFunc);
}

void MainWidget::ShowAddItemForm()
{
    ProductItemForm* form = new ProductItemForm(nullptr, [this](ProductItem createdProductItem){

        qDebug() << "Data: " << createdProductItem.ToQString();

        if (_networkManager->VerifyProduct(createdProductItem)){

            _customer->AddToQuery(createdProductItem);
        }

    });

    form->show();
}

void MainWidget::ShowEditItemForm()
{

    ProductItem editableItem = _customer->GetSelected();

    if (editableItem.IsDefault()){

        return;
    }
    else {

        ProductItemForm* form = new ProductItemForm(editableItem, nullptr, [this](ProductItem editedProductItem){

            qDebug() << "Data: " << editedProductItem.ToQString();

            _customer->UpdateProduct(editedProductItem);

        });

        form->show();

    }






}

void MainWidget::RemoveItemFromQuery(){

    _customer->RemoveSelectedFromQuery();
}

void MainWidget::ClearQuery()
{
    _customer->ClearQuery();
}

MainWidget::~MainWidget()
{

}

