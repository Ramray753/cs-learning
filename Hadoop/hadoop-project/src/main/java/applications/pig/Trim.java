package applications.pig;

import org.apache.pig.PrimitiveEvalFunc;

public class Trim extends PrimitiveEvalFunc<String, String> {
    @Override
    public String exec(String input) {
        return input.trim();
    }
}