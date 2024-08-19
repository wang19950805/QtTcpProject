使用QTcpSocket构建服务端和客户端的通信  
![image]()  ![image](https://github.com/wang19950805/QtTcpProject/blob/master/cilent.jpg)


server:  

     QNetworkInterface::allAddresses()             //获取server上的所有地址
     nextPendingConnection()                       //获取新的客户端连接  
     stateChanged(QAbstractSocket::SocketState)    //QTcpSocket的信号,获取tcp的连接状态  

cilent:   

      connectToHost()                              //连接服务端,需要传递ip地址和端口两个参数
      errorOccurred(QAbstractSocket::SocketError)  //QTcpSocket的信号,获取tcp的连接的异常状态   


需要注意:     

      1.发送信号应在连接完成之后的槽函数中进行,可通过SocketState中的QAbstractSocket::ConnectedState进行判断.    
      2.多个客户端进行连接服务器时,需注意区分不同客户端的端口号下表索引,通过qobject_cast<QTcpSocket *>(sender())获取当前的tcp连接   
        调用peerPort()方法获取当前客户端的端口进行信息传递.  
      3.页面部分应在各种操作执行中,或执行后,对相应的组件进行禁用处理,防止误操作打乱业务逻辑和tcp的重复连接.  
      
