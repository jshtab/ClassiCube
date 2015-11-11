﻿using System;
using System.Drawing;
using OpenTK.Input;

namespace ClassicalSharp {
	
	public sealed partial class AltTextInputWidget : Widget {

		public AltTextInputWidget( Game game, Font font, Font boldFont, TextInputWidget parent ) : base( game ) {
			HorizontalAnchor = Anchor.LeftOrTop;
			VerticalAnchor = Anchor.BottomOrRight;
			this.font = font;
			this.boldFont = boldFont;
			this.parent = parent;
		}
		
		public Texture texture;
		readonly Font font, boldFont;
		TextInputWidget parent;
		Size elementSize;
		
		public bool Active;
		public void SetActive( bool active ) {
			Active = active;
			Height = active ? texture.Height : 0;
		}
		
		public override void Render( double delta ) {
			texture.Render( graphicsApi );
		}
		
		public override void Init() {
			X = 5; Y = 5;
			InitData();
			Redraw();
		}

		public void Redraw() {
			Make( elements[selectedIndex], font );
			Width = texture.Width;
			Height = texture.Height;		
		}
		
		unsafe void Make( Element e, Font font ) {
			Size* sizes = stackalloc Size[e.Contents.Length / e.CharsPerItem];
			MeasureContentSizes( e, font, sizes );
			Size bodySize = CalculateContentSize( e, sizes, out elementSize );
			int titleWidth = MeasureTitles( font ), titleHeight = elements[0].TitleSize.Height;
			Size size = new Size( Math.Max( bodySize.Width, titleWidth ), bodySize.Height + titleHeight );
			
			using( Bitmap bmp = IDrawer2D.CreatePow2Bitmap( size ) ) {
				using( IDrawer2D drawer = game.Drawer2D ) {
					drawer.SetBitmap( bmp );
					DrawTitles( drawer, font );
					drawer.Clear( new FastColour( 30, 30, 30, 200 ), 0, titleHeight,
					             size.Width, bodySize.Height );
					
					DrawContent( drawer, font, e, titleHeight );
					texture = drawer.Make2DTexture( bmp, size, X, Y );
				}
			}
		}
		
		int selectedIndex = 0;
		public override bool HandlesMouseClick( int mouseX, int mouseY, MouseButton button ) {
			mouseX -= X; mouseY -= Y;
			if( IntersectsHeader( mouseX, mouseY ) ) {
				Dispose();
				Redraw();
			} else {
				IntersectsBody( mouseX, mouseY );
			}
			return true;
		}
		
		bool IntersectsHeader( int widgetX, int widgetY ) {
			Rectangle bounds = new Rectangle( 0, 0, 0, 0 );
			for( int i = 0; i < elements.Length; i++ ) {
				Size size = elements[i].TitleSize;
				bounds.Width = size.Width; bounds.Height = size.Height;
				if( bounds.Contains( widgetX, widgetY ) ) {
					selectedIndex = i;
					return true;
				}
				bounds.X += size.Width;
			}
			return false;
		}
		
		void IntersectsBody( int widgetX, int widgetY ) {
			widgetY -= elements[0].TitleSize.Height;
			widgetX /= elementSize.Width; widgetY /= elementSize.Height;
			Element e = elements[selectedIndex];
			int index = widgetY * e.ItemsPerRow + widgetX;
			if( index < e.Contents.Length ) {
				if( selectedIndex == 0 ) {
					// TODO: need to insert characters that don't affect caret index, adjust caret colour
					//parent.AppendChar( e.Contents[index * e.CharsPerItem] );
					//parent.AppendChar( e.Contents[index * e.CharsPerItem + 1] );
				} else {
					parent.AppendChar( e.Contents[index] );
				}
			}
		}

		public override void Dispose() {
			graphicsApi.DeleteTexture( ref texture );
		}
	}
}