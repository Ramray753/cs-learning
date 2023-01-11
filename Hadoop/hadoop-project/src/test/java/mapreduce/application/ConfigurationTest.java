package mapreduce.application;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.util.Tool;
import org.junit.Test;

import java.util.Arrays;

public class ConfigurationTest {
    @Test
    public void addResource() {
        Configuration conf = new Configuration();
        /*
        Add a configuration resource.
        The properties of this resource will override properties of previously added resources,
        unless they were marked final.
         */
        conf.addResource("configuration-1.xml");
        System.out.println("color: " + conf.get("color"));
        System.out.println("size: " + conf.getInt("size", 0));
        System.out.println("size-weight: " + Arrays.toString(conf.getStrings("size-weight")));
        System.out.println("not-exist: " + conf.get("not-exist", "nan"));
    }

    @Test
    public void addResource2() {
        Configuration conf = new Configuration();
        conf.addResource("configuration-1.xml");
        conf.addResource("configuration-2.xml");
        System.out.println("size: " + conf.getInt("size", 0));
        System.out.println("weight: " + conf.get("weight"));
        System.out.println("size-weight: " + Arrays.toString(conf.getStrings("size-weight")));
    }

    @Test
    public void sysConfig() {
        Configuration conf = new Configuration();
        conf.addResource("configuration-1.xml");
        System.out.println("size-weight: " + Arrays.toString(conf.getStrings("size-weight")));
        System.setProperty("size", "123");
        System.out.println("size-weight: " + Arrays.toString(conf.getStrings("size-weight")));
    }
    enum MyCounter {
        PRIME
    }
    @Test
    public void test(){
        System.out.println(MyCounter.PRIME.getDeclaringClass().getName());
        System.out.println(MyCounter.PRIME.name());
    }


}
