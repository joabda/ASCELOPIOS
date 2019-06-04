package fbeventplugin;

import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.SpringLayout;
import javax.swing.JTextField;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JButton;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class MainWindow extends JFrame {

	private JPanel contentPane;
	private JTextField textField;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					MainWindow frame = new MainWindow();
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the frame.
	 */
	public MainWindow() {
		setTitle("Facebook Event Plugin - v1.0");
		setResizable(false);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 316, 130);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		SpringLayout sl_contentPane = new SpringLayout();
		contentPane.setLayout(sl_contentPane);
		
		textField = new JTextField();
		sl_contentPane.putConstraint(SpringLayout.WEST, textField, 15, SpringLayout.WEST, contentPane);
		sl_contentPane.putConstraint(SpringLayout.EAST, textField, 221, SpringLayout.WEST, contentPane);
		contentPane.add(textField);
		textField.setColumns(10);
		
		JLabel lblFacebookEventLink = new JLabel("Facebook event link:");
		sl_contentPane.putConstraint(SpringLayout.NORTH, lblFacebookEventLink, 5, SpringLayout.NORTH, contentPane);
		sl_contentPane.putConstraint(SpringLayout.NORTH, textField, 11, SpringLayout.SOUTH, lblFacebookEventLink);
		sl_contentPane.putConstraint(SpringLayout.WEST, lblFacebookEventLink, 10, SpringLayout.WEST, contentPane);
		contentPane.add(lblFacebookEventLink);
		
		JButton btnAdd = new JButton("Add");
		sl_contentPane.putConstraint(SpringLayout.NORTH, btnAdd, -3, SpringLayout.NORTH, textField);
		sl_contentPane.putConstraint(SpringLayout.EAST, btnAdd, -10, SpringLayout.EAST, contentPane);
		contentPane.add(btnAdd);
		
		btnAdd.addActionListener(new ActionListener() { 
			  public void actionPerformed(ActionEvent e) { 
				    String fbeventlink = textField.getText();
				    if (!fbeventlink.contains("www.facebook.com/events/")) {
				    	JOptionPane.showMessageDialog(null, "The link isn't correct. Please insert the full link.", "Error", JOptionPane.ERROR_MESSAGE);
				    } else {
				    	FBParser fbp = new FBParser();
				    	if (!fbp.addEvent(fbeventlink))
				    		JOptionPane.showMessageDialog(null, "Ops, something has gone wrong. Is the link correct?", "Error", JOptionPane.ERROR_MESSAGE);
				    	else
				    		System.exit(0);
				    }
				  } 
				} );
		
		JLabel lblHelp = new JLabel("Note: first you must create a category called 'facebook'");
		sl_contentPane.putConstraint(SpringLayout.WEST, lblHelp, 0, SpringLayout.WEST, textField);
		sl_contentPane.putConstraint(SpringLayout.SOUTH, lblHelp, -10, SpringLayout.SOUTH, contentPane);
		lblHelp.setFont(new Font("Dialog", Font.ITALIC, 10));
		contentPane.add(lblHelp);
	}
}
