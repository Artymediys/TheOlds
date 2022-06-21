import javax.swing.*;

public class Main
{
	public static void main(String[] args)
	{
		JFrame f = new JFrame();

		f.setSize(400, 400);
		f.setVisible(true);

		f.add(new Individual21(f));
	}
}