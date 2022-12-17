#include "MessDispalyWidget.h"
#include "PhotoWidget.h"
#include "FileMonitorWidget.h"
#include <QFileDialog>
#include <QSplitter>
#include <QStatusBar>
#include <QDirIterator>
#include <QFile>


MessDispalyWidget::MessDispalyWidget(const QString &db_file, const QString &table_name){

    initConnectSql(db_file, table_name);
    initExePath();
    QSplitter *mainwidget = new QSplitter(this);

    QGroupBox *message_box = initMessBox();
    QGroupBox *file_box = initFileBox();
    initMenu();
    mainwidget->addWidget(file_box);
    mainwidget->addWidget(message_box);
    this->setCentralWidget(mainwidget);
}


void MessDispalyWidget::AutoMerge(){
    if(!this->exe_map.contains("root") || this->exe_map.value("root")==""){
        QMessageBox::warning(this,"提示","请先设置根路径！");
        return;
    }else if(!QFile::exists(this->exe_map.value("root"))){
        QMessageBox::warning(this,"提示","根路径不存在，请先设置根路径");
        return;
    }else{
        QDir ROOT = QDir(this->exe_map.value("root"));
        QMessageBox::StandardButton result=QMessageBox::question(this, "提示", "所有病人资料移动至"+ this->exe_map.value("root") +".\n该操作不可逆，是否继续？");
        if(result == QMessageBox::No)
        {
            return;
        }
        QDir TMPDir;
        QStringList sucess_pats;
        QStringList fail_pats;
        for(int i=0; i<this->m_model->rowCount();i++)
        {
            QSqlRecord record = m_model->record(i);
            QString pat_root = record.value("file_root").toString();
            QString pat_name = record.value("name").toString();
            if (pat_root =="" || !QFile::exists(this->exe_map.value("root"))){
                continue;
            }else{
                QString base_name = QFileInfo(pat_root).baseName();
                QString dest_root = ROOT.absoluteFilePath(base_name);
                if(pat_root!=dest_root && QDir(pat_root).exists()){
                    bool x = TMPDir.rename(pat_root, dest_root);
                    if(x){
                        sucess_pats.append(pat_name);
                        record.setValue("file_root",dest_root);
                        this->m_model->setRecord(i,record);
                        this->m_model->submitAll();
                    }else{
                        fail_pats.append(pat_name);
                    }
                }
            }
         }
        this->m_model->select();
        QMessageBox::information(this,"提示","移动成功："+ QString::number(sucess_pats.size())+"\n移动失败:" + QString::number(fail_pats.size()));
        if(fail_pats.size()>0){
            QMessageBox::StandardButton result=QMessageBox::question(this, "提示", "是否查看移动失败文件？");
            if(result == QMessageBox::Yes)
            {
                QMessageBox::information(this,"移动失败文件",fail_pats.join(","));
            }
        }
        return;
    }
}


void MessDispalyWidget::AutoClassify(){
    if(!this->exe_map.contains("root") || this->exe_map.value("root")==""){
        QMessageBox::warning(this,"提示","请先设置根路径！");
        return;
    }else if(!QFile::exists(this->exe_map.value("root"))){
        QMessageBox::warning(this,"提示","根路径不存在，请先设置根路径");
        return;
    }else{
        QDir ROOT = QDir(this->exe_map.value("root"));
        QMessageBox::StandardButton result=QMessageBox::question(this, "提示", "自动识别"+ this->exe_map.value("root") +"下的文件，并将其添加到表中，是否继续？");
        if(result == QMessageBox::No)
        {
            return;
        }
        ROOT.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        ROOT.setSorting(QDir::Name);
        QFileInfoList all_dirs = ROOT.entryInfoList();
        QStringList hasAdded;
        QStringList hasBinded;
        QList<int> needAdd;


        for(int i=0; i<all_dirs.size(); ++i){
            QString pat_name = all_dirs.at(i).baseName();
            QString add_dir = all_dirs.at(i).absoluteFilePath();
            if (this->patHasExist(pat_name)){
                hasAdded<< pat_name;
                continue;
            }else{
                QString binded_pat_name = this->fileRootHasBinded(add_dir);
                if (binded_pat_name!=""){
                    hasBinded << add_dir;
                }else{
                needAdd.append(i);
                }
            }
        }

        int maxcount = this->m_model->rowCount();
        int insert_rows = needAdd.size();

        if(this->m_model->insertRows(maxcount,insert_rows)){
            int name_index = this->m_model->fieldIndex("name");
            int root_index = this->m_model->fieldIndex("file_root");
            for(int j=0; j<needAdd.size(); ++j){
                int dir_index = needAdd.at(j);
                QString pat_name = all_dirs.at(dir_index).baseName();
                QString pat_file_root = all_dirs.at(dir_index).absoluteFilePath();

                for (int col=0; col< this->m_model->columnCount(); ++col){
                    this->m_model->setData(this->m_model->index(maxcount+j,col),"");
                }
                this->m_model->setData(this->m_model->index(maxcount+j,name_index),pat_name);
                this->m_model->setData(this->m_model->index(maxcount+j,root_index),pat_file_root);
                this->m_model->submitAll();
            }
        }
        QStringList infoShow;
        infoShow << "共添加：" << QString::number(this->m_model->rowCount()-maxcount)<<"条信息。";
        if (hasAdded.size()>0){
            infoShow << "\n跳过【" << hasAdded.join("、") << "】，因为表中已存在。" ;
        }
        if (hasBinded.size()>0){
            infoShow << "\n跳过【" << hasBinded.join("、") << "】，因为路径已被绑定。" ;
        }
        QMessageBox::information(this,"提示",infoShow.join(""));
        returnAllTable();
        return;
    }
}


void MessDispalyWidget::SetExePath(const QString file_name){
    QString current_path = this->exe_map.value(file_name);
    if (current_path!="" && QFile::exists(current_path)){
        QMessageBox::StandardButton result=QMessageBox::question(this, "提示", "当前设置路径为：" + current_path+",确定重新设置吗？");
        if(result == QMessageBox::No)
        {
            return;
        }
    }
    QString file_root="";
    if (file_name == "root"){
        file_root = QFileDialog::getExistingDirectory(this,"设置根路径");
    }else{
        file_root = QFileDialog::getOpenFileName(this, "选择"+ file_name+ "打开方式位置",QCoreApplication::applicationDirPath(),"*.exe");
    }
    if(file_root!=""){
        this->exe_map[file_name] = file_root;
        QFile file(this->program_path.absolutePath());
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        foreach(const QString & file_class, this->need_add_exe_path){
            out<<file_class<<this->exe_map.value(file_class);
        }
        file.close();
        QMessageBox::information(this,"提示","设置成功！");
    }
    return;
}


void MessDispalyWidget::initExePath(){
    this->program_path = QDir(QCoreApplication::applicationDirPath() + "/" + _EXE_PATH);
    QFile file(this->program_path.absolutePath());
    if (file.exists()){
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);
        while(!in.atEnd()){
            QString key;
            QString value;
            in >> key >> value;
            this->exe_map.insert(key,value);
        }
    }else{
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        foreach(const QString & file_class, this->need_add_exe_path){
            this->exe_map.insert(file_class,"");
            out<<file_class<<"";
        }
    }
    file.close();
}


void MessDispalyWidget::initConnectSql(const QString &db_file, const QString &table_name){
    if(QSqlDatabase::contains("qt_sql_default_connection")){
         m_db = QSqlDatabase::database("qt_sql_default_connection");
    }else{
         m_db = QSqlDatabase::addDatabase("QSQLITE");
    }
    m_db.setDatabaseName(db_file);
    if(!m_db.open()){
        qDebug()<<"打开失败";
        return;
    }

    m_model = new QSqlTableModel(this, m_db);

    //设置数据表
    m_model->setTable(table_name);
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    //查询数据
    if(!m_model->select()){
        qDebug()<<"查询失败";
        return;
    }
    m_model->setHeaderData(m_model->fieldIndex("name"),Qt::Horizontal,"姓名");
    m_model->setHeaderData(m_model->fieldIndex("sex"),Qt::Horizontal,"性别");
    m_model->setHeaderData(m_model->fieldIndex("age"),Qt::Horizontal,"年龄");
    m_model->setHeaderData(m_model->fieldIndex("id_number"),Qt::Horizontal,"身份证号");
    m_model->setHeaderData(m_model->fieldIndex("outpatient_number"),Qt::Horizontal,"就诊号");
    m_model->setHeaderData(m_model->fieldIndex("file_root"),Qt::Horizontal,"资料路径");

    this->m_viewer = new QTableView;
    this->m_viewer->setModel(m_model);
    this->m_viewer->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->m_viewer->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->m_viewer->resizeColumnsToContents();
    this->m_viewer->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->m_viewer->setColumnHidden(m_model->fieldIndex("file_root"), true);
    this->m_viewer->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    // 当表的内容有修改的时候，释放出修改信号
    QObject::connect(this,&MessDispalyWidget::tableChanged,this,&MessDispalyWidget::findPatMess);

}


QGroupBox * MessDispalyWidget::initMessBox(){
    QGroupBox * box = new QGroupBox("病人信息");
    QVBoxLayout * layout = new QVBoxLayout();
    QGridLayout * Buts = new QGridLayout;
    AddPatBtn = new QPushButton("新增");
    DelPatBtn = new QPushButton("删除");
    ModifyBtn = new QPushButton("修改");
    ReturnBtn = new QPushButton("返回");
    FindPatEdit = new QLineEdit;
    FindPatEdit->setPlaceholderText("输入病人相关信息查找...");
    FindPatEdit->setClearButtonEnabled(true);
    FindPatEdit->setStyleSheet("QLineEdit{padding:2,2,2,2;}");


    SetFileBtn = new QPushButton("设置路径");
    DelFileBtn = new QPushButton("删除路径");
    ShowFileBtn = new QPushButton("显示路径");

    Buts->addWidget(FindPatEdit,0,0,1,2);
    Buts->addWidget(ReturnBtn,0,2);
    Buts->addWidget(AddPatBtn,1,0);
    Buts->addWidget(DelPatBtn,1,1);
    Buts->addWidget(ModifyBtn,1,2);
    Buts->addWidget(SetFileBtn,2,0);
    Buts->addWidget(DelFileBtn,2,1);
    Buts->addWidget(ShowFileBtn,2,2);

    layout->addWidget(this->m_viewer,10);
    layout->addLayout(Buts,1);


    // 增，弹出对话框，填充信息
    QObject::connect(AddPatBtn,&QPushButton::clicked,this,&MessDispalyWidget::AddPatBox);
    // 删，接受row信号，删除某一行
    QObject::connect(DelPatBtn,&QPushButton::clicked,this,&MessDispalyWidget::deleteData);
    // 改，弹出对话框（显示已有信息），在对话框上修改后，通过提交信号，修改原数据
    QObject::connect(ModifyBtn,&QPushButton::clicked,this,&MessDispalyWidget::modifyBox);
    // 查，模糊查询，只信息查询结果
    QObject::connect(FindPatEdit,&QLineEdit::returnPressed,this,&MessDispalyWidget::findPatMess);
    QObject::connect(ReturnBtn,&QPushButton::clicked,this,&MessDispalyWidget::returnAllTable);

    // 增，弹出对话框，填充信息
    QObject::connect(SetFileBtn,&QPushButton::clicked,this,&MessDispalyWidget::SetFileBox);
    // 删，接受row信号，删除某一行
    QObject::connect(DelFileBtn,&QPushButton::clicked,this,&MessDispalyWidget::DeleteFileBox);
    // 显示当前病人文件路径
    QObject::connect(ShowFileBtn,&QPushButton::clicked,this,&MessDispalyWidget::ShowFileBox);


    QObject::connect(this->m_viewer->selectionModel(),SIGNAL(currentRowChanged(const QModelIndex &,const QModelIndex &)),this,SLOT(changePatFile(const QModelIndex &,const QModelIndex &)));
    QObject::connect(this,&MessDispalyWidget::fileRootChange,this,&MessDispalyWidget::flushTableBox);


    box->setLayout(layout);
    return box;
}


QGroupBox * MessDispalyWidget::initFileBox(){


    QGroupBox * box = new QGroupBox("文件信息");
    FileMonitorWidget * file_widget = new FileMonitorWidget();
    this->m_file_layout =  new QVBoxLayout;
    this->m_file_layout->addWidget(file_widget);
    this->m_file = file_widget;
    box->setLayout(this->m_file_layout);
    return box;
}


void MessDispalyWidget::initMenu(){

    QStatusBar *statusbar = new QStatusBar(this);
    this->setStatusBar(statusbar);

    menubar= new QMenuBar(this);
    menubar->setStyleSheet("QMenuBar{background-color:transparent;} "
                           "QMenuBar::selected{background-color:rgba(100,100,100,1);}"
                           "QMenuBar::item{font-size:16px;font-family:Microsoft YaHei; Spacing:10px; padding: 0px 5px 15px 5px;}");

    this->setMenuBar(menubar);//构建菜单栏
    QMenu *file_menu=new QMenu("文件",this);
    QMenu *add_path_menu=new QMenu("路径设置",this);

    menubar->addSeparator();
    menubar->addMenu(file_menu);
    menubar->addMenu(add_path_menu);

    file_menu->setStyleSheet("QMenu::item {min-width:50px;font-size: 16px}"
                             "QMenu::item:selected {background:rgba(82,130,164,1);border:1px solid rgba(82,130,164,1)}");
    add_path_menu->setStyleSheet("QMenu::item {min-width:50px;font-size: 16px}"
                         "QMenu::item:selected {background:rgba(82,130,164,1);border:1px solid rgba(82,130,164,1)}");


    QAction *autoClassify;
    autoClassify = new QAction(QIcon(":/myIcon/icon/icons8-shazam-50.png"),"批量添加", this);
    autoClassify->setStatusTip("自动将根路径下的文件名作为病人名添加到表中，并绑定路径");
    file_menu->addAction(autoClassify);
    QObject::connect(autoClassify, &QAction::triggered, this, &MessDispalyWidget::AutoClassify);

    QAction *autoMerge;
    autoMerge = new QAction(QIcon(":/myIcon/icon/icons8-categorize-24.png"),"一键归档", this);
    autoMerge->setStatusTip("将所有病人资料整理至根路径");
    file_menu->addAction(autoMerge);
    QObject::connect(autoMerge, &QAction::triggered, this, &MessDispalyWidget::AutoMerge);


    signalMapper = new QSignalMapper(this);
    foreach(const QString & file_class, this->need_add_exe_path){
        QAction *tmpAct;
        if (file_class == "root"){
            tmpAct = new QAction(QIcon(":/myIcon/icon/icons8-folder-30.png"),"根路径", this);
            tmpAct->setStatusTip("设置根路径");
        }else{
            tmpAct = new QAction(QIcon(":/myIcon/icon/icons8-exe-24.png"),file_class, this);
            tmpAct->setStatusTip("设置"+file_class+"打开方式");
        }
        add_path_menu->addAction(tmpAct);
        signalMapper->setMapping(tmpAct, file_class);
        QObject::connect(tmpAct, SIGNAL (triggered()), signalMapper, SLOT(map()));
    }
    QObject::connect(signalMapper, SIGNAL(mapped(QString)),this, SLOT(SetExePath(QString)));
}


void MessDispalyWidget::changePatFile(const QModelIndex &index, const QModelIndex &){
    int row = index.row();
    QSqlRecord record = m_model->record(row);
    QString current_file_root = record.value("file_root").toString();
    emitSignal(current_file_root);
}


void MessDispalyWidget::flushTableBox(QString file_root){
    FileMonitorWidget * tmp_widget = this->m_file;
    this->m_file_layout->removeWidget(tmp_widget);
    delete tmp_widget;
    FileMonitorWidget * new_file_widget = new FileMonitorWidget(file_root, this->exe_map);
    this->m_file_layout->addWidget(new_file_widget);
    this->m_file = new_file_widget;
    return;
}


void MessDispalyWidget::SetFileBox(){
    int modify_row = this->get_selected_one_row();
    if(modify_row<0){
        return;
    }
    QSqlRecord record = m_model->record(modify_row);
    QString current_file_root = record.value("file_root").toString();
    if(current_file_root != "" && QDir(current_file_root).exists()){
        QStringList showinfo;
        showinfo << "当前路径:" <<current_file_root<<",确定要重新设置吗？";
        QMessageBox::StandardButton result=QMessageBox::question(this, "提示", showinfo.join(""));
        if(result == QMessageBox::No)
        {
            return;
        }
    }
    QString file_root;
    if(this->exe_map.value("root") !="" && QFile::exists(this->exe_map.value("root"))){
        file_root = QFileDialog::getExistingDirectory(this, "选择文件路径", this->exe_map.value("root"));
    }else{
         file_root = QFileDialog::getExistingDirectory(this, "选择文件路径");
    }
    while(1){
        if (file_root == ""){
            return;
        }else{
            QString file_root_owner = this->fileRootHasBinded(file_root);
            if (file_root_owner!=""){
             QMessageBox::StandardButton result=QMessageBox::question(this, "提示","该路径已经被设置为"+file_root_owner+"的文件路径,是否重新设置路径！");
             if(result == QMessageBox::No){
                 return;
             }else{
                 QDir next_start = QDir(file_root);
                 next_start.cdUp();
                 file_root = QFileDialog::getExistingDirectory(this, tr("选择重新文件路径"),next_start.absolutePath());
                 continue;
             }
            }
            else{
                record.setValue("file_root",file_root);
                this->m_model->setRecord(modify_row, record);
                m_model->submitAll();
                emitSignal(file_root);
                QMessageBox::information(this,"提示","设置成功");
                return;
            }
        }
    }
}


void MessDispalyWidget::DeleteFileBox(){
    int modify_row = this->get_selected_one_row();
    if(modify_row<0){
        return;
    }
    QSqlRecord record = m_model->record(modify_row);
    QString current_file_root = record.value("file_root").toString();

    if(current_file_root != ""){
        QStringList showinfo;
        showinfo << "当前路径" << current_file_root;
        if (!QDir(current_file_root).exists()){
            showinfo << "为无效路径";
        }
        showinfo.append(",是否删除？");
        QMessageBox::StandardButton result=QMessageBox::question(this, "提示",showinfo.join(""));
        if(result == QMessageBox::No)
        {
            return;
        }else{
            record.setValue("file_root","");
            this->m_model->setRecord(modify_row, record);
            m_model->submitAll();
            emitSignal("");
            QMessageBox::information(this,"提示","删除成功");
        }
    }else{
       QMessageBox::warning(this,"提示","已经是空路径");
    }
}


void MessDispalyWidget::ShowFileBox(){
    int row = this->get_selected_one_row();
    if(row<0){
        return;
    }
    QSqlRecord record = m_model->record(row);
    QString current_file_root = record.value("file_root").toString();
    if (current_file_root == ""){
        QMessageBox::warning(this, "当前路径", "当前路径为空");
    }else if (!QDir(current_file_root).exists()){
         QMessageBox::critical(this, "提示", "无效路径:"+current_file_root);
         DeleteFileBox();
    }else{
         QMessageBox::information(this, "当前路径", current_file_root);
    }
}


void MessDispalyWidget::AddPatBox(){
    this->returnAllTable();
    AddNewPatDialog * add_box = new AddNewPatDialog;
    QObject::connect(add_box,&AddNewPatDialog::closeBox,this,&MessDispalyWidget::insertData);
    add_box->exec();
    delete add_box;
}


void MessDispalyWidget::insertData(const Patient &patient){
    QSqlRecord record = m_model->record();
    QHash<QString,QString>::const_iterator i;

    for(i= patient.info_map.constBegin(); i!=patient.info_map.constEnd(); ++i){
        record.setValue(i.key(),i.value());
    }
    m_model->insertRecord(0,record);//每次都从第一行插入
    m_model->submitAll();//如果设置保存策略是OnManualSubmit，则需要每次都提交一次修改
    m_model->select();//每次对数据库有什么变动，都应该重新筛选一次，获取最新的数据
}


void MessDispalyWidget::deleteData(){

    QList<int> selected_rows = this->get_selected_rows();
    if (selected_rows.size()<=0){
        return;
    }
    QStringList delete_row_str1;
    foreach(const int row, selected_rows){
        delete_row_str1.append(QString::number(row+1));
    }
    QString delete_row_str2 = delete_row_str1.join(",");
    for (int i = selected_rows.size()-1; i>=0;  i--)
    {
        this->m_model->removeRow(selected_rows.at(i));
    }
    QMessageBox::StandardButton result=QMessageBox::question(this, "删除","确定要删除第"+ delete_row_str2 +"行数据吗？");
    if(result == QMessageBox::Yes)
    {
        m_model->submitAll();
        emitTableChangedSignal();
        emitSignal("");
    }else{
        m_model->revertAll();//回退，注意，只有在保存策略是OnManualSubmit时有用
    }
    m_model->select();
}


void MessDispalyWidget::modifyBox(){
    int modify_row = this->get_selected_one_row();
    if(modify_row<0){
        return;
    }
    Patient current_pat;
    QSqlRecord record = m_model->record(modify_row);
    QHash<QString, QString> tmp_info_map;
    QHash<QString,QString>::const_iterator i;

    for(i= current_pat.info_map.constBegin(); i!=current_pat.info_map.constEnd(); ++i){
        tmp_info_map.insert(i.key(), record.value(i.key()).toString());
    }
    current_pat.set_info_map(tmp_info_map);

    ModifyPatDialog *modify_box = new ModifyPatDialog(current_pat);
    QObject::connect(modify_box,&ModifyPatDialog::closeBox,this,&MessDispalyWidget::modifyData);
    modify_box->exec();
    delete modify_box;
}


void MessDispalyWidget::modifyData(const Patient& pat){
    QItemSelectionModel *model_selection = this->m_viewer->selectionModel();
    QModelIndexList index_list= model_selection->selectedIndexes();
    int modify_row = index_list.at(0).row();
    QSqlRecord record = m_model->record(modify_row);
    QHash<QString,QString>::const_iterator i;
    for(i= pat.info_map.constBegin(); i!=pat.info_map.constEnd(); ++i){
        record.setValue(i.key(),i.value());
    }
    this->m_model->setRecord(modify_row, record);

    QMessageBox::StandardButton result=QMessageBox::question(this, "修改","确定要修改吗？");
    if(result == QMessageBox::Yes)
    {
        m_model->submitAll();
    }else{
        m_model->revertAll();
    }
    m_model->select();
}


void MessDispalyWidget::findPatMess(){
    // 去除用户输入的空格等信息
    QString text = this->FindPatEdit->text();
    text.remove(QRegExp("\\s"));
    QList<int> find_results;

    for(int i=0;i<this->m_model->rowCount();i++)
    {
        QString r="";
        QModelIndex index;
        // 不需要进行搜索的内容
        QSet<int> skipFind = { this->m_model->fieldIndex("file_root")};
        for(int j=0;j<this->m_model->columnCount();j++)
        {
            index=this->m_model->index(i,j);
            if (skipFind.contains(j)){
                continue;
            }
            r+=this->m_model->data(index).toString();
        }
        r.remove(QRegExp("\\s"));
        if(r.contains(text,Qt::CaseSensitive)){
            find_results.append(i);
        }
    }

    if (find_results.size()<=0){
        QMessageBox::critical(this, "提示","无相关信息");
        this->returnAllTable();
        return;
    }else{
        for(int i=0;i<this->m_model->rowCount();i++)
        {
            this->m_viewer->setRowHidden(i,true);
        }
        foreach(int i, find_results)
        {
            this->m_viewer->setRowHidden(i,false);
        }
    }
}


QString MessDispalyWidget::fileRootHasBinded(const QString &file_root){

    for(int i=0; i<this->m_model->rowCount();i++)
    {
        QSqlRecord record = m_model->record(i);
        QString hasBindedRoot = record.value("file_root").toString();

        if(file_root==hasBindedRoot || getSubFolders(hasBindedRoot).contains(file_root))
        {
            return record.value("name").toString();
        }
     }
    return "";
}


QSet<QString> MessDispalyWidget::getSubFolders(QString path)
{
    //判断路径是否存在
    QSet<QString> result;
    QDir dir(path);
    if(path == "" || !dir.exists()){
        return result;
    }

    //定义迭代器并设置过滤器
    QDirIterator dir_iterator(path, QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QStringList string_list;
    while(dir_iterator.hasNext())
    {
        dir_iterator.next();
        QFileInfo file_info = dir_iterator.fileInfo();
        result.insert(file_info.absoluteFilePath());
    }
    return result;
}

bool MessDispalyWidget::patHasExist(const QString pat_name){
    QSet<QString> pats;

    for(int i=0; i<this->m_model->rowCount();i++)
    {
        QSqlRecord record = m_model->record(i);
        pats.insert(record.value("name").toString());
    }
    if(pats.contains(pat_name)){
        return true;
    }else{
        return false;
    }
}


void MessDispalyWidget::returnAllTable(){
    for(int i=0;i<this->m_model->rowCount();i++)
    {
        this->m_viewer->setRowHidden(i,false);
    }
}


int MessDispalyWidget::get_selected_one_row(){
    QItemSelectionModel *model_selection = this->m_viewer->selectionModel();
    QModelIndexList index_list= model_selection->selectedIndexes();
    if (index_list.size() <=0){
        QMessageBox::warning(this,"提示","请选择病人");
        return -1;
    }
    QHash<int,int>row_map;
    foreach (QModelIndex index, index_list)
    {
        if(!index.isValid()) return -1;
        if(index.column()!=0) continue;
        row_map.insert(index.row(),0);
    }

    QList<int> modify_rows = row_map.keys();
    if (modify_rows.size() > 1 ){
        QMessageBox::warning(this,"提示","只能选择一个病人");
        return -1;
    }
    int modify_row = index_list.at(0).row();
    return modify_row;
}


QList<int> MessDispalyWidget::get_selected_rows(){
    QList<int> selected_rows;
    QItemSelectionModel *model_selection = this->m_viewer->selectionModel();
    QModelIndexList index_list= model_selection->selectedIndexes();
    if (index_list.size() <=0){
        QMessageBox::warning(this,"提示","请选择要删除的病人");
        return selected_rows;
    }
    QMap<int,int>row_map;
    foreach (QModelIndex index, index_list)
    {
        if(!index.isValid()) return selected_rows;
        if(index.column()!=0) continue;
        row_map.insert(index.row(),0);
    }
    selected_rows = row_map.keys();
    return selected_rows;
}


MessDispalyWidget::~MessDispalyWidget(){
    delete m_model;
    delete m_viewer;
    delete m_file;
    delete m_file_layout;

    delete AddPatBtn;
    delete DelPatBtn;
    delete ModifyBtn;
    delete ReturnBtn;
    delete FindPatEdit;
    delete SetFileBtn;
    delete DelFileBtn;
    delete ShowFileBtn;

}


