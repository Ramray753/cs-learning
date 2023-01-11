package applications.hbase;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.HColumnDescriptor;
import org.apache.hadoop.hbase.HTableDescriptor;
import org.apache.hadoop.hbase.TableName;
import org.apache.hadoop.hbase.client.*;
import org.apache.hadoop.hbase.util.Bytes;

import java.io.IOException;

/**
 * HBase 1.0 version of ExampleClient that uses {@code Connection},
 * {@code Admin} and {@code Table}.
 */
public class NewExampleClient {

    public static void main(String[] args) throws IOException {
        // 创建配置类，用于创建连接
        Configuration config = HBaseConfiguration.create();
        Connection connection = ConnectionFactory.createConnection(config);
        try {
            // Create table
            // 获取管理员实例
            Admin admin = connection.getAdmin();
            try {
                // 表名
                TableName tableName = TableName.valueOf("test");
                // 表模式
                HTableDescriptor htd = new HTableDescriptor(tableName);
                // 列族
                HColumnDescriptor hcd = new HColumnDescriptor("data");
                // 向模式中添加表模式
                htd.addFamily(hcd);
                // 根据表模式来创建表
                admin.createTable(htd);
                // 获取用户空间中所有表
                HTableDescriptor[] tables = admin.listTables();
                // 检查刚才是否创建成功
                // tableName.getName()返回的是Byte[]：HBase中的数据都是字节，只有在解释的时候才会被转化成相应的类型！
                // 默认代码运行之前之前没有任何表被创建
                if (tables.length != 1 && Bytes.equals(tableName.getName(), tables[0].getTableName().getName())) {
                    throw new IOException("Failed create of table");
                }
                // Run some operations -- three puts, a get, and a scan -- against the table.
                // 通过表名获取表
                Table table = connection.getTable(tableName);
                try {
                    // put: 添加数据
                    for (int i = 1; i <= 3; i++) {
                        byte[] row = Bytes.toBytes("row" + i);
                        // 创建行，传入行键
                        Put put = new Put(row);
                        // 列族
                        byte[] columnFamily = Bytes.toBytes("data");
                        // 列族修饰词
                        byte[] qualifier = Bytes.toBytes(String.valueOf(i));
                        // 值
                        byte[] value = Bytes.toBytes("value" + i);
                        // 装载数据
                        put.addColumn(columnFamily, qualifier, value);
                        // 添加行
                        table.put(put);
                    }
                    // get：查询数据
                    // 通过行键获取行
                    Get get = new Get(Bytes.toBytes("row1"));
                    Result result = table.get(get);
                    System.out.println("Get: " + result);
                    // scan：扫描表
                    Scan scan = new Scan();
                    ResultScanner scanner = table.getScanner(scan);
                    try {
                        for (Result scannerResult : scanner) {
                            System.out.println("Scan: " + scannerResult);
                        }
                    } finally {
                        scanner.close();
                    }
                    // Disable then drop the table
                    // 禁用并删除表
                    admin.disableTable(tableName);
                    admin.deleteTable(tableName);
                } finally {
                    table.close();
                }
            } finally {
                admin.close();
            }
        } finally {
            connection.close();
        }
    }
}