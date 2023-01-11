package crunch;

import org.apache.crunch.PCollection;
import org.apache.crunch.PTable;
import org.apache.crunch.impl.mem.MemPipeline;
import org.apache.crunch.types.writable.Writables;
import org.junit.Test;

import static crunch.Utils.dump;

public class LibTest {
    @Test
    public void test1() {
        PCollection<String> a = MemPipeline.typedCollectionOf(Writables.strings(), "cherry", "apple", "banana", "banana");

        System.out.println(a.length().getValue());
        System.out.println(a.min().getValue());
        System.out.println(a.max().getValue());

        PTable<String, Long> b = a.count();
        System.out.println(dump(b));

        PTable<String, Long> c = b.top(1);
        System.out.println(dump(c));

        PTable<String, Long> d = b.bottom(2);
        System.out.println(dump(d));
    }
}
