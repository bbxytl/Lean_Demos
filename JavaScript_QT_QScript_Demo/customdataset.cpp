#include "customdataset.h"



CustomDataSet::CustomDataSet(QObject * parent) : NCReportDataSource( parent )
{
    setDataSourceType(Custom);
    setLocationType(Static);
    recno() =0;
}

CustomDataSet::~ CustomDataSet()
{
    m_lsVar.clear();
}

bool CustomDataSet::open()
{
    if ( m_lsVar.isEmpty() ) {
        error()->setError(QString("No data in CustomDataSet datasource") );
        return false;
    }
    recno() =0;
    setOpened(true);
    return true;
}

bool CustomDataSet::close()
{
    recno() =0;
    setOpened(false);
    return true;
}

bool CustomDataSet::next()
{
    recno()++;

    if ( recno() >= m_lsVar.count() ) {
        recno()--;
        flagEnd() = true;
        return false;
    }
    flagBegin() = false;
    return true;
}

int CustomDataSet::size() const
{
    return m_lsVar.count();
}

bool CustomDataSet::previous()
{
    if ( recno() <= 0 ) {
        recno() = 0;
        flagBegin() = true;
        return false;
    } else {
        recno()--;
    }
    return true;
}

bool CustomDataSet::first()
{
    recno()=0;
    return true;
}

bool CustomDataSet::last()
{
    recno() = m_lsVar.count()-1;
    return true;
}

QVariant CustomDataSet::value(const QString & column, bool* ok, int i ) const
{
    int col = columnIndexByName(column);
    if (col<0) {
        *ok = false;
        return QVariant();
    }
    return value( col, ok, i );
}

QVariant CustomDataSet::value( int column, bool*, int ) const
{
    QVariant v;
    if(column<m_lsVar.at(recno()).size())
        v=m_lsVar.at(recno()).at(column);
    return v;
}

bool CustomDataSet::isValid() const
{
    return true;
}

bool CustomDataSet::read(NCReportXMLReader *)
{
    return true;
}

bool CustomDataSet::write(NCReportXMLWriter *)
{
    return true;
}

void CustomDataSet::evaluate(NCReportEvaluator *)
{
}

void CustomDataSet::addData(const QList<QVariantList> &datas)
{
    foreach (QVariantList da, datas) {
        m_lsVar.append( da );
    }

}

void CustomDataSet::insertData(const QVariantList &data, const int pos)
{
    if(pos!=-1 && pos<m_lsVar.size())
        m_lsVar.insert(pos,data);
    else
        m_lsVar.append(data);
}

void CustomDataSet::insertCol(const QString &columnName, const int pos)
{
    if(pos!=-1 && pos<m_colName.size())
        m_colName.insert(pos,columnName);
    else
        m_colName.append(columnName);
}

QVariant CustomDataSet::getData(const int row, const int col)
{
    if(row<0||row>rowCount())return QVariant();
    if(col<0||col>columnCount())return QVariant();
    return m_lsVar.at(row).at(col);
}

bool CustomDataSet::modifyData(const int row, const int col, const QVariant &data)
{
    try{
        if(row<0||row>rowCount())return false;
        if(col<0||col>columnCount())return false;
        QVariantList tmp=m_lsVar.at(row);
        tmp.replace(col,data);
        m_lsVar.replace(row,tmp);
    }catch(...)
    {
        return false;
    }
    return true;
}

bool CustomDataSet::modifyCol( const int col, const QString &data)
{
    if(col<0||col>columnCount())return false;
    m_colName.replace(col,data);
    return true;
}

bool CustomDataSet::modifyCol(const QString &colNameOld,
                              const QString &colNameNew)
{
    try{
        unsigned int sz=m_colName.size();
        for(unsigned int i=0;i<sz;++i) {
            if(colNameOld==m_colName.at(i))
                m_colName.replace(i,colNameNew);
        }
    }catch(...)
    {
        return false;
    }
    return true;
}

QVariantList CustomDataSet::removeData(const int pos)
{
    try{
        QVariantList rs;
        if(pos==-1){
            rs=m_lsVar.last();
            m_lsVar.removeLast();
        }else{
            rs=m_lsVar.at(pos);
            m_lsVar.removeAt(pos);
        }
        return rs;
    }catch(...){
        return QList<QVariant>();
    }
}

QList<QVariantList> *CustomDataSet::getCustomData()
{
    return &m_lsVar;
}

void CustomDataSet::clear()
{
    m_lsVar.clear();
    m_colName.clear();
}

void CustomDataSet::addCols(const QStringList &cols)
{
    foreach (QString columnName, cols) {
        m_colName.append(columnName);
    }
}

bool CustomDataSet::hasNext()
{
    return (recno()<m_lsVar.count()-1);
}

bool CustomDataSet::seek( int index )
{
    bool ok=true;
    if ( index == -1 )
        recno() =0;
    else if ( index < m_lsVar.count())
        recno() = index;
    else
    {
        recno() = m_lsVar.count()-1;
        ok=false;
    }

    return ok;
}

int CustomDataSet::columnIndexByName(const QString &columnname) const
{
    unsigned int sz=columnCount();
    for (unsigned int i=0; i<sz; ++i) {
        if (columnname==columnName(i))
            return i;
    }

    return -1;
}

int CustomDataSet::columnCount() const
{
    return m_colName.size();
}

int CustomDataSet::rowCount() const
{
    return m_lsVar.size();
}

QString CustomDataSet::columnName(int column) const
{
    if(column<m_colName.size())
        return m_colName.at(column);

    return QString();
}


