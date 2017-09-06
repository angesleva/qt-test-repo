import QtQuick 2.4
import QtQuick.Controls 2.1
import HRApp.models.employees 1.0
import HRApp.models.contractTypes 1.0

ComboBox {

    id: contractTypes
    currentIndex: 0
    property alias included: contractTypeModel.allIncluded
    property alias contractTypes: contractTypes

    signal contractTypeChanged(int index)

    model: ContractTypeModel {
        id: contractTypeModel
    }

    Connections {
        target: contractTypes
        onActivated: { //Signal whenever selection changes.
            contractTypeChanged(index)
        }
    }
}
