import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.Arrays;
import java.util.List;

public class Individual21 extends JComponent
{
	private record Vector2i(int x, int y)
	{
		public Vector2i(Point p)
		{
			this(p.x, p.y);
		}

		public Vector2i translate(int tx, int ty)
		{
			return new Vector2i(this.x + tx, this.y + ty);
		}

		public Vector2i rotate(float r)
		{
			float sin = (float) Math.sin(r);
			float cos = (float) Math.cos(r);

			return new Vector2i((int) (cos * this.x - sin * this.y), (int) (cos * this.y + sin * this.x));
		}
	}

	private interface IShape
	{
		void draw(Graphics2D g);

		IShape translate(int tx, int ty);

		IShape rotate(float r);
	}

	private record Circle(Vector2i c, int r) implements IShape
	{
		@Override
		public void draw(Graphics2D g)
		{
			circle(g, this.c.x(), this.c.y(), this.r);
		}

		@Override
		public IShape translate(int tx, int ty)
		{
			return new Circle(this.c.translate(tx, ty), this.r);
		}

		@Override
		public IShape rotate(float r)
		{
			return new Circle(this.c.rotate(r), this.r);
		}
	}

	private record Polygon(Vector2i... points) implements IShape
	{
		@Override
		public void draw(Graphics2D g)
		{
			for(int i = 0; i < this.points.length; ++i)
			{
				Vector2i v1 = this.points[i];
				Vector2i v2 = this.points[(i + 1) % this.points.length];
				line(g, v1.x, v1.y, v2.x, v2.y);
			}
		}

		@Override
		public IShape translate(int tx, int ty)
		{
			Vector2i[] points = new Vector2i[this.points.length];

			for(int i = 0; i < this.points.length; ++i)
			{
				points[i] = this.points[i].translate(tx, ty);
			}

			return new Polygon(points);
		}

		@Override
		public IShape rotate(float r)
		{
			Vector2i[] points = new Vector2i[this.points.length];

			for(int i = 0; i < this.points.length; ++i)
			{
				points[i] = this.points[i].rotate(r);
			}

			return new Polygon(points);
		}
	}

	private static final List<IShape> SHAPES = Arrays.asList(
			new Polygon(new Vector2i(200, 50), new Vector2i(300, 150), new Vector2i(200, 250), new Vector2i(100, 150)),
			new Polygon(new Vector2i(100, 150), new Vector2i(100, 250), new Vector2i(200, 250)),
			new Circle(new Vector2i(100, 150), 50));

	private static final Vector2i SHAPE_CENTER = new Vector2i(100, 150);

	private boolean mode;
	private Vector2i rotationPos;
	private float rotation;
	private Vector2i translationPos;

	public Individual21(JFrame f)
	{
		f.addMouseListener(new MouseListener()
		{
			@Override
			public void mouseClicked(MouseEvent e)
			{
				Individual21.this.update(e.getButton(), new Vector2i(e.getPoint()));
			}

			@Override
			public void mousePressed(MouseEvent e) {}

			@Override
			public void mouseReleased(MouseEvent e) {}

			@Override
			public void mouseEntered(MouseEvent e) {}

			@Override
			public void mouseExited(MouseEvent e) {}
		});
	}

	private void update(int btn, Vector2i clickedPos)
	{
		if(btn == 3)
		{
			this.mode = !this.mode;
			return;
		}

		if(btn != 1)
		{
			return;
		}

		if(this.mode)
		{
			this.translationPos = clickedPos;
		}
		else
		{
			this.rotationPos = clickedPos;
			this.rotation += (float) Math.PI / 8f;
		}

		this.repaint();
	}

	@Override
	public void paint(Graphics g)
	{
		Graphics2D g2d = (Graphics2D) g;

		for(IShape shape : SHAPES)
		{
			if(this.translationPos != null)
			{
				shape = shape.translate(-SHAPE_CENTER.x() + this.translationPos.x(), -SHAPE_CENTER.y() + this.translationPos.y());
			}

			if(this.rotationPos != null)
			{
				shape = shape.translate(-this.rotationPos.x(), -this.rotationPos.y());
				shape = shape.rotate(this.rotation);
				shape = shape.translate(this.rotationPos.x(), this.rotationPos.y());
			}

			shape.draw(g2d);
		}
	}

	/*
	 * Drawing
	 */

	public static void pixel(Graphics2D g, int x, int y)
	{
		g.fillRect(x, y, 1, 1);
	}

	// Bresenham
	public static void line(Graphics2D g, int x1, int y1, int x2, int y2)
	{
		int dx = x2 - x1;
		int dy = y2 - y1;
		int absdx = Math.abs(dx);
		int absdy = Math.abs(dy);

		int x = x1;
		int y = y1;

		// slope < 1
		if(absdx > absdy)
		{
			int d = 2 * absdy - absdx;

			for(int i = 0; i < absdx; ++i)
			{
				x = dx < 0 ? x - 1 : x + 1;

				if(d < 0)
				{
					d = d + 2 * absdy;
				}
				else
				{
					y = dy < 0 ? y - 1 : y + 1;
					d = d + (2 * absdy - 2 * absdx);
				}

				pixel(g, x, y);
			}
		}
		else
		{
			int d = 2 * absdx - absdy;

			for(int i = 0; i < absdy; ++i)
			{
				y = dy < 0 ? y - 1 : y + 1;

				if(d < 0)
				{
					d = d + 2 * absdx;
				}
				else
				{
					x = dx < 0 ? x - 1 : x + 1;
					d = d + (2 * absdx) - (2 * absdy);
				}

				pixel(g, x, y);
			}
		}
	}

	public static void octants(Graphics2D g, int xc, int yc, int x, int y)
	{
		pixel(g, xc + x, yc + y);
		pixel(g, xc - x, yc + y);
		pixel(g, xc + x, yc - y);
		pixel(g, xc - x, yc - y);
		pixel(g, xc + y, yc + x);
		pixel(g, xc - y, yc + x);
		pixel(g, xc + y, yc - x);
		pixel(g, xc - y, yc - x);
	}

	public static void circle(Graphics2D g, int xc, int yc, int r)
	{
		int x = 0;
		int y = r;

		int d = 3 - 2 * r;

		octants(g, xc, yc, x, y);
		while (y >= x)
		{
			x++;

			if (d > 0)
			{
				y--;
				d = d + 4 * (x - y) + 10;
			}
			else
			{
				d = d + 4 * x + 6;
			}

			octants(g, xc, yc, x, y);
		}
	}
}